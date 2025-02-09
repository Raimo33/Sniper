/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix_serializer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 15:36:24 by craimond          #+#    #+#             */
/*   Updated: 2025/02/02 15:36:27 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/fix_serializer.h"

static bool message_fits_in_buffer(const fix_message_t *message, const uint16_t buffer_size);
static uint8_t compute_checksum(const char *buffer, const uint16_t len);
static inline bool is_checksum_sequence(const char *buffer);
static const char *get_checksum_start(const char *buffer, const uint16_t buffer_size);
static void tokenize_message(char *buffer, const uint16_t buffer_size);
static inline uint16_t check_begin_string(const char *buffer);
static inline uint16_t check_content_length_tag(const char *buffer);
static inline uint16_t deserialize_content_length(const char *buffer, uint16_t *content_length);
static inline uint16_t validate_checksum(char *buffer, const uint16_t buffer_size, const uint16_t content_length, char **body_start);

uint16_t serialize_fix_message(char *buffer, const uint16_t buffer_size, const fix_message_t *message)
{
  fast_assert(buffer && message, "Unexpected NULL pointer");

  const char *buffer_start = buffer;

  if (!message_fits_in_buffer(message, buffer_size))
    panic("FIX message does not fit in buffer");

  const fix_field_t *fields = message->fields;
  const uint16_t n_fields = message->n_fields;
  for (uint8_t i = 0; LIKELY(i < n_fields); i++)
  {
    memcpy(buffer, fields[i].tag, fields[i].tag_len);
    buffer += fields[i].tag_len;
    *buffer++ = '=';

    memcpy(buffer, fields[i].value, fields[i].value_len);
    buffer += fields[i].value_len;
    *buffer++ = '\x01';
  }

  return buffer - buffer_start;
}

uint16_t finalize_fix_message(char *buffer, const uint16_t buffer_size, const uint16_t len)
{
  fast_assert(buffer, "Unexpected NULL pointer");
  const char *buffer_start = buffer;

  static const fix_field_t begin_string = { STR_AND_LEN(FIX_BEGINSTRING), STR_AND_LEN(FIX_VERSION) };

  char body_length_str[16];
  const fix_field_t body_length = { STR_AND_LEN(FIX_BODYLENGTH), body_length_str, ultoa(len, body_length_str) };

  const uint8_t added_len = begin_string.tag_len + 1 + begin_string.value_len + 1 + body_length.tag_len + 1 + body_length.value_len + 1;
  const uint8_t checksum_len = STR_LEN(FIX_CHECKSUM) + 1 + 3 + 1;
  fast_assert(len + added_len + checksum_len <= buffer_size, "FIX message does not fit in buffer");

  memmove(buffer + added_len, buffer, len);

  const fix_message_t message = {
    .fields = {
      begin_string,
      body_length
    },
    .n_fields = 2
  };
  buffer += serialize_fix_message(buffer, added_len, &message);

  static const char checksum_table[256][sizeof(uint32_t)] = {
    {"000\x01"}, {"001\x01"}, {"002\x01"}, {"003\x01"}, {"004\x01"}, {"005\x01"}, {"006\x01"}, {"007\x01"}, {"008\x01"}, {"009\x01"},
    {"010\x01"}, {"011\x01"}, {"012\x01"}, {"013\x01"}, {"014\x01"}, {"015\x01"}, {"016\x01"}, {"017\x01"}, {"018\x01"}, {"019\x01"},
    {"020\x01"}, {"021\x01"}, {"022\x01"}, {"023\x01"}, {"024\x01"}, {"025\x01"}, {"026\x01"}, {"027\x01"}, {"028\x01"}, {"029\x01"},
    {"030\x01"}, {"031\x01"}, {"032\x01"}, {"033\x01"}, {"034\x01"}, {"035\x01"}, {"036\x01"}, {"037\x01"}, {"038\x01"}, {"039\x01"},
    {"040\x01"}, {"041\x01"}, {"042\x01"}, {"043\x01"}, {"044\x01"}, {"045\x01"}, {"046\x01"}, {"047\x01"}, {"048\x01"}, {"049\x01"},
    {"050\x01"}, {"051\x01"}, {"052\x01"}, {"053\x01"}, {"054\x01"}, {"055\x01"}, {"056\x01"}, {"057\x01"}, {"058\x01"}, {"059\x01"},
    {"060\x01"}, {"061\x01"}, {"062\x01"}, {"063\x01"}, {"064\x01"}, {"065\x01"}, {"066\x01"}, {"067\x01"}, {"068\x01"}, {"069\x01"},
    {"070\x01"}, {"071\x01"}, {"072\x01"}, {"073\x01"}, {"074\x01"}, {"075\x01"}, {"076\x01"}, {"077\x01"}, {"078\x01"}, {"079\x01"},
    {"080\x01"}, {"081\x01"}, {"082\x01"}, {"083\x01"}, {"084\x01"}, {"085\x01"}, {"086\x01"}, {"087\x01"}, {"088\x01"}, {"089\x01"},
    {"090\x01"}, {"091\x01"}, {"092\x01"}, {"093\x01"}, {"094\x01"}, {"095\x01"}, {"096\x01"}, {"097\x01"}, {"098\x01"}, {"099\x01"},
    {"100\x01"}, {"101\x01"}, {"102\x01"}, {"103\x01"}, {"104\x01"}, {"105\x01"}, {"106\x01"}, {"107\x01"}, {"108\x01"}, {"109\x01"},
    {"110\x01"}, {"111\x01"}, {"112\x01"}, {"113\x01"}, {"114\x01"}, {"115\x01"}, {"116\x01"}, {"117\x01"}, {"118\x01"}, {"119\x01"},
    {"120\x01"}, {"121\x01"}, {"122\x01"}, {"123\x01"}, {"124\x01"}, {"125\x01"}, {"126\x01"}, {"127\x01"}, {"128\x01"}, {"129\x01"},
    {"130\x01"}, {"131\x01"}, {"132\x01"}, {"133\x01"}, {"134\x01"}, {"135\x01"}, {"136\x01"}, {"137\x01"}, {"138\x01"}, {"139\x01"},
    {"140\x01"}, {"141\x01"}, {"142\x01"}, {"143\x01"}, {"144\x01"}, {"145\x01"}, {"146\x01"}, {"147\x01"}, {"148\x01"}, {"149\x01"},
    {"150\x01"}, {"151\x01"}, {"152\x01"}, {"153\x01"}, {"154\x01"}, {"155\x01"}, {"156\x01"}, {"157\x01"}, {"158\x01"}, {"159\x01"},
    {"160\x01"}, {"161\x01"}, {"162\x01"}, {"163\x01"}, {"164\x01"}, {"165\x01"}, {"166\x01"}, {"167\x01"}, {"168\x01"}, {"169\x01"},
    {"170\x01"}, {"171\x01"}, {"172\x01"}, {"173\x01"}, {"174\x01"}, {"175\x01"}, {"176\x01"}, {"177\x01"}, {"178\x01"}, {"179\x01"},
    {"180\x01"}, {"181\x01"}, {"182\x01"}, {"183\x01"}, {"184\x01"}, {"185\x01"}, {"186\x01"}, {"187\x01"}, {"188\x01"}, {"189\x01"},
    {"190\x01"}, {"191\x01"}, {"192\x01"}, {"193\x01"}, {"194\x01"}, {"195\x01"}, {"196\x01"}, {"197\x01"}, {"198\x01"}, {"199\x01"},
    {"200\x01"}, {"201\x01"}, {"202\x01"}, {"203\x01"}, {"204\x01"}, {"205\x01"}, {"206\x01"}, {"207\x01"}, {"208\x01"}, {"209\x01"},
    {"210\x01"}, {"211\x01"}, {"212\x01"}, {"213\x01"}, {"214\x01"}, {"215\x01"}, {"216\x01"}, {"217\x01"}, {"218\x01"}, {"219\x01"},
    {"220\x01"}, {"221\x01"}, {"222\x01"}, {"223\x01"}, {"224\x01"}, {"225\x01"}, {"226\x01"}, {"227\x01"}, {"228\x01"}, {"229\x01"},
    {"230\x01"}, {"231\x01"}, {"232\x01"}, {"233\x01"}, {"234\x01"}, {"235\x01"}, {"236\x01"}, {"237\x01"}, {"238\x01"}, {"239\x01"},
    {"240\x01"}, {"241\x01"}, {"242\x01"}, {"243\x01"}, {"244\x01"}, {"245\x01"}, {"246\x01"}, {"247\x01"}, {"248\x01"}, {"249\x01"},
    {"250\x01"}, {"251\x01"}, {"252\x01"}, {"253\x01"}, {"254\x01"}, {"255\x01"}
  };

  const uint8_t checksum = compute_checksum(buffer_start, buffer - buffer_start);

  memcpy(buffer, STR_AND_LEN(FIX_CHECKSUM));
  buffer += STR_LEN(FIX_CHECKSUM);
  *buffer++ = '=';
  *(uint32_t *)buffer = *(const uint32_t *)checksum_table[checksum];
  buffer += 4;

  return buffer - buffer_start;
}

bool is_full_fix_message(const char *buffer, UNUSED const uint16_t buffer_size, const uint16_t message_len)
{
  static const uint8_t checksum_len = STR_LEN(FIX_CHECKSUM "=000\x01");
  static const uint8_t begin_string_len = STR_LEN(FIX_BEGINSTRING "=" FIX_VERSION "\x01");
  static const uint8_t body_length_len = STR_LEN(FIX_BODYLENGTH "=");

  if (message_len < (checksum_len + begin_string_len + body_length_len))
    return false;

  //TODO potenziale loop infinito se content len e' piu grante del buffer_size. fare un assert

  return (!!get_checksum_start(buffer, message_len));
}

static const char *get_checksum_start(const char *buffer, const uint16_t buffer_size)
{
  const char *end = buffer + buffer_size - STR_LEN(FIX_CHECKSUM "=000\x01");

#ifdef __AVX512F__
  while (UNLIKELY(buffer + 64 < end))
  {
    __m512i chunk = _mm512_loadu_si512((const void*)(buffer));

    __mmask64 m1 = _mm512_cmpeq_epi8_mask(chunk, _mm512_set1_epi8('1'));
    __mmask64 m2 = _mm512_cmpeq_epi8_mask(chunk, _mm512_set1_epi8('0'));
    __mmask64 m3 = _mm512_cmpeq_epi8_mask(chunk, _mm512_set1_epi8('='));
    __mmask64 m4 = _mm512_cmpeq_epi8_mask(chunk, _mm512_set1_epi8(0x01));

    __mmask64 candidates = _kand_mask64(m1, _kshiftri_mask64(m2, 1));
    candidates = _kand_mask64(candidates, _kshiftri_mask64(m3, 2));
    candidates = _kand_mask64(candidates, _kshiftri_mask64(m4, 6));

    if (candidates)
    {
      int32_t index = __builtin_ctzll(candidates);
      return buffer + index;
    }

    buffer += 64;
  }
#endif

#ifdef __AVX2__
  while (UNLIKELY(buffer + 32 < end))
  {
    __m256i chunk = _mm256_loadu_si256((const void*)(buffer));

    __m256i m1 = _mm256_cmpeq_epi8(chunk, _mm256_set1_epi8('1'));
    __m256i m2 = _mm256_cmpeq_epi8(chunk, _mm256_set1_epi8('0'));
    __m256i m3 = _mm256_cmpeq_epi8(chunk, _mm256_set1_epi8('='));
    __m256i m4 = _mm256_cmpeq_epi8(chunk, _mm256_set1_epi8(0x01));

    __m256i shifted_m2 = _mm256_srli_epi64(m2, 1);
    __m256i shifted_m3 = _mm256_srli_epi64(m3, 2);
    __m256i shifted_m4 = _mm256_srli_epi64(m4, 6);

    __m256i candidates = _mm256_and_si256(m1, shifted_m2);
    candidates = _mm256_and_si256(candidates, shifted_m3);
    candidates = _mm256_and_si256(candidates, shifted_m4);

    int mask = _mm256_movemask_epi8(candidates);
    if (mask)
    {
      int index = __builtin_ctz(mask);
      return buffer + index;
    }

    buffer += 32;
  }
#endif

#ifdef __SSE2__
  while (UNLIKELY(buffer + 16 < end))
  {
    __m128i chunk = _mm_loadu_si128((const __m128i*)(buffer));

    __m128i m1 = _mm_cmpeq_epi8(chunk, _mm_set1_epi8('1'));
    __m128i m2 = _mm_cmpeq_epi8(chunk, _mm_set1_epi8('0'));
    __m128i m3 = _mm_cmpeq_epi8(chunk, _mm_set1_epi8('='));
    __m128i m4 = _mm_cmpeq_epi8(chunk, _mm_set1_epi8(0x01));

    __m128i shifted_m2 = _mm_srli_epi64(m2, 1);
    __m128i shifted_m3 = _mm_srli_epi64(m3, 2);
    __m128i shifted_m4 = _mm_srli_epi64(m4, 6);

    __m128i candidates = _mm_and_si128(m1, shifted_m2);
    candidates = _mm_and_si128(candidates, shifted_m3);
    candidates = _mm_and_si128(candidates, shifted_m4);

    int32_t mask = _mm_movemask_epi8(candidates);
    if (mask)
    {
      int16_t index = __builtin_ctz(mask);
      return buffer + index;
    }

    buffer += 16;
  }
#endif

  while (LIKELY(buffer < end))
  {
    if (is_checksum_sequence(buffer))
      return buffer;
    buffer++;
  }

  return NULL;
}

static inline bool is_checksum_sequence(const char *buffer)
{
  uint64_t word = *(const uint64_t *)buffer;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  const uint64_t expected = ('1' <<  0) | ('0' <<  8) | ('=' << 16) | (0x01ULL << 55);
  const uint64_t mask = (0xFFULL <<  0) | (0xFFULL <<  8) | (0xFFULL << 16) | (0xFFULL << 55);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  const uint64_t expected = ('1' << 56) | ('0' << 48) | ('=' << 40) | (0x01ULL << 8);
  const uint64_t mask = (0xFFULL << 56) | (0xFFULL << 48) | (0xFFULL << 40) | (0xFFULL << 8);
#else
  # error "Unknown byte order"
#endif

  return (word & mask) == expected;
}

uint16_t deserialize_fix_message(char *restrict buffer, const uint16_t buffer_size, fix_message_t *restrict message)
{
  fast_assert(buffer && message, "Unexpected NULL pointer");
  
  const char *buffer_start = buffer;
  char *body_start;
  uint16_t content_length;

  buffer += check_begin_string(buffer);
  buffer += check_content_length_tag(buffer);
  buffer += deserialize_content_length(buffer, &content_length);  
  buffer += validate_checksum(buffer, buffer_size, content_length, &body_start);

  tokenize_message(body_start, content_length);
  //TODO fill the message fields

  return buffer - buffer_start;
}

static inline uint16_t check_begin_string(const char *buffer)
{
  static const char begin_string_tag[] = FIX_BEGINSTRING "=" FIX_VERSION "\x01";
  if (UNLIKELY(*(uint16_t *)buffer != *(const uint16_t *)begin_string_tag))
    panic("Invalid FIX begin string tag");
  return STR_LEN(begin_string_tag);
}

static inline uint16_t check_content_length_tag(const char *buffer)
{
  static const char body_length_tag[] = FIX_BODYLENGTH "=";
  if (UNLIKELY(memcmp(buffer, body_length_tag, STR_LEN(body_length_tag))))
    panic("Invalid FIX body length tag");
  return STR_LEN(body_length_tag);
}

static inline uint16_t deserialize_content_length(const char *buffer, uint16_t *content_length)
{
  const char *buffer_start = buffer;

  *content_length = (uint16_t)strtoul(buffer, (char **)&buffer, 10);
  fast_assert(*buffer == '\x01', "Missing field separator");
  buffer += STR_LEN("\x01");

  return buffer - buffer_start;
}

static inline uint16_t validate_checksum(char *buffer, const uint16_t buffer_size, const uint16_t content_length, char **body_start)
{
  *body_start = buffer;

  const char *body_end = get_checksum_start(buffer, buffer_size);
  const char *checksum_start = body_end + STR_LEN(FIX_CHECKSUM "=");
  fast_assert(body_end - *body_start == content_length, "Misleading content length provided");

  const uint8_t expected_checksum = compute_checksum(*body_start, content_length);
  const uint8_t provided_checksum = (uint8_t)strtoul(checksum_start, (char **)&buffer, 10);
  fast_assert(expected_checksum == provided_checksum, "Checksum mismatch");
  buffer += STR_LEN("\x01");

  return buffer - *body_start;
}

static void tokenize_message(char *restrict buffer, const uint16_t buffer_size)
{
  const char *end = buffer + buffer_size;

#ifdef __AVX512F__
  {
    __m512i vec_soh = _mm512_set1_epi8(0x01);
    __m512i vec_equals = _mm512_set1_epi8(0x3D);

    while (LIKELY(buffer + 64 <= end))
    {
      __m512i chunk = _mm512_loadu_si512((__m512i*)buffer);
      __m512i cmp_soh = _mm512_cmpeq_epi8(chunk, vec_soh);
      __m512i cmp_equals = _mm512_cmpeq_epi8(chunk, vec_equals);
      __m512i cmp = _mm512_or_si512(cmp_soh, cmp_equals);
      __m512i result = _mm512_andnot_si512(cmp, chunk);
      _mm512_storeu_si512((__m512i*)buffer, result);
      buffer += 64;
    }
  }
#endif

#ifdef __AVX2__
  {
    __m128i vec_soh = _mm_set1_epi8(0x01);
    __m128i vec_equals = _mm_set1_epi8(0x3D);
  
    while (LIKELY(buffer + 16 <= end))
    {
      __m128i chunk = _mm_loadu_si128((__m128i*)buffer);
      
      __m128i cmp_soh = _mm_cmpeq_epi8(chunk, vec_soh);
      __m128i cmp_equals = _mm_cmpeq_epi8(chunk, vec_equals);
      __m128i cmp = _mm_or_si128(cmp_soh, cmp_equals);
      
      __m128i result = _mm_andnot_si128(cmp, chunk);
      _mm_storeu_si128((__m128i*)buffer, result);
      
      buffer += 16;
    }
  }
#endif

  static const uint64_t mask_soh = 0x0101010101010101ULL;
  static const uint64_t mask_equals = 0x3D3D3D3D3D3D3D3DULL;
  static const uint64_t ones = 0x0101010101010101ULL;
  static const uint64_t high_mask = 0x8080808080808080ULL;
  
  while (LIKELY(buffer + 8 <= end))
  {
    const uint64_t chunk = *(const uint64_t *)buffer;

    const uint64_t tmp_soh = ((chunk ^ mask_soh) - ones) & ~(chunk ^ mask_soh) & high_mask;
    const uint64_t tmp_equals = ((chunk ^ mask_equals) - ones) & ~(chunk ^ mask_equals) & high_mask;

    const uint64_t matching_tokens = tmp_soh | tmp_equals;
    const uint64_t byte_mask = ((matching_tokens >> 7) & ones) * 0xFF;

    const uint64_t new_chunk = chunk & ~byte_mask;
    *((uint64_t *)buffer) = new_chunk;

    buffer += 8;
  }

  while (LIKELY(buffer < end))
  {
    const char c = *buffer;
    *buffer = (c != '\x01' || c != '=') * c;
    buffer++;
  }
}

static bool message_fits_in_buffer(const fix_message_t *restrict message, const uint16_t buffer_size)
{
  uint16_t total_len = (message->n_fields << 1);

  for (uint8_t i = 0; LIKELY(i < message->n_fields); i++)
    total_len += message->fields[i].tag_len + message->fields[i].value_len;

  return total_len <= buffer_size;
}

static uint8_t compute_checksum(const char *buffer, const uint16_t len)
{
  uint8_t checksum = 0;
  
  const char *end = buffer + len;

#ifdef __AVX512F__
  while (LIKELY(buffer + 64 <= end))
  {
    __m512i vec = _mm512_loadu_si512((const __m512i *)buffer);
    __m512i sum = _mm512_sad_epu8(vec, _mm512_setzero_si512());
    
    checksum += (uint8_t)_mm512_reduce_add_epi64(sum);
    buffer += 64;
  }
#endif

#ifdef __AVX2__
  while (LIKELY(buffer + 32 <= end))
  {
    __m256i vec = _mm256_loadu_si256((const __m256i *)buffer);
    __m256i sum = _mm256_sad_epu8(vec, _mm256_setzero_si256());
    
    __m128i sum_low = _mm256_castsi256_si128(sum);
    __m128i sum_high = _mm256_extracti128_si256(sum, 1);
    sum_low = _mm_add_epi64(sum_low, sum_high);

    checksum += (uint8_t)(_mm_extract_epi64(sum_low, 0) + _mm_extract_epi64(sum_low, 1));
    buffer += 32;
  }
#endif

#ifdef __SSE2__
  while (LIKELY(buffer + 16 <= end))
  {
    __m128i vec = _mm_loadu_si128((const __m128i *)buffer);
    __m128i sum = _mm_sad_epu8(vec, _mm_setzero_si128());

    checksum += (uint8_t)(_mm_extract_epi64(sum, 0) + _mm_extract_epi64(sum, 1));
    buffer += 16;
  }
#endif

  while (LIKELY(buffer < end))
    checksum += *buffer++;

  return checksum;
}
