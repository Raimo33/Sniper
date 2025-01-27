/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:57:09 by craimond          #+#    #+#             */
/*   Updated: 2025/01/27 19:27:07 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/http_parser.h"

static const str_len_pair_t methods[] = {
  [HTTP_GET]    {STR_LEN_PAIR("GET")},
  [HTTP_POST]   {STR_LEN_PAIR("POST")},
  [HTTP_PUT]    {STR_LEN_PAIR("PUT")},
  [HTTP_DELETE] {STR_LEN_PAIR("DELETE")}
};
static const str_len_pair_t versions[] = {
  [HTTP_1_0] {STR_LEN_PAIR("HTTP/1.0")},
  [HTTP_1_1] {STR_LEN_PAIR("HTTP/1.1")}
};

static void HOT strlower(char *str, uint16_t len);
static void HOT header_map_insert(header_map_t *restrict map, const char *restrict key, const uint16_t key_len, const char *restrict value, const uint16_t value_len);

void build_http_request(const http_request_t *restrict req, char *restrict buf)
{
  memcpy(buf, methods[req->method].str, methods[req->method].len);
  buf += methods[req->method].len;

  *buf++ = ' ';
  
  memcpy(buf, req->path, req->path_len);
  buf += req->path_len;

  *buf++ = ' ';

  memcpy(buf, versions[req->version].str, versions[req->version].len);
  buf += versions[req->version].len;

  static const uint16_t colon_space = (uint16_t)':' << sizeof(char) | ' ';
  static const uint16_t crlf = (uint16_t)'\r' << sizeof(char) | '\n';

  const header_entry_t *headers = req->headers;

  #pragma ivdep
  for (uint8_t i = 0; LIKELY(i < HEADER_MAP_SIZE); i++)
  {
    PREFETCHR(&headers[i + 1], L0);

    memcpy(buf, headers[i].key, headers[i].key_len);
    buf += headers[i].key_len;
    
    *(uint16_t *)buf = colon_space;
    buf += 2;

    memcpy(buf, headers[i].value, headers[i].value_len);
    buf += headers[i].value_len;
    
    *(uint16_t *)buf = crlf;
    buf += 2;
  }

  *(uint16_t *)buf = crlf;
  buf += 2;

  memcpy(buf, req->body, req->body_len);
  buf += req->body_len;
}

bool parse_http_response(char *restrict buf, const uint16_t buf_size, http_response_t *restrict res, const uint16_t res_len)
{
  assert(res_len <= buf_size, STR_LEN_PAIR("Malformed HTTP response: response length exceeds buffer size"));
  assert(buf_size >= 4, STR_LEN_PAIR("Buffer too small for HTTP response"));

  const char *headers_end = memmem(buf, buf_size, "\r\n\r\n", 4);
  if (UNLIKELY(!headers_end))
    return false;
  if (UNLIKELY(res->body))
    goto parse_body;

  char *line = memchr(buf, ' ', headers_end - buf);
  assert(line, STR_LEN_PAIR("Malformed HTTP response: missing status code"));
  line += 1;

  res->status_code = atoi(line);
  assert(res->status_code >= 100 && res->status_code <= 599, STR_LEN_PAIR("Malformed HTTP response: invalid status code"));

  char *key, *value;
  uint8_t key_len, value_len;
  char *delim = memmem(buf, buf_size, "\r\n", 2);
  *delim = '\0';
  line = delim + 2;
  while (LIKELY(*line != '\r'))
  {
    key = line;

    delim = memchr(line, ':', headers_end - line);
    assert(delim, STR_LEN_PAIR("Malformed HTTP response: missing header"));

    *delim = '\0';
    value = delim + 1 + (delim[1] == ' ');

    delim = memmem(value, headers_end - value, "\r\n", 2);
    *delim = '\0';
    line = delim + 1 + (delim[1] == ' ');

    key_len = key - line - 2;
    value_len = delim - value;

    strlower(key, key_len);

    if (UNLIKELY(memcmp(key, "content-length", key_len) == 0))
      res->body_len = atoi(value);
    else if (UNLIKELY(memcmp(key, "transfer-encoding", key_len) == 0))
      panic(STR_LEN_PAIR("Transfer-Encoding not supported"));

    header_map_insert(&res->headers, key, key_len, value, value_len);
  }

  if (LIKELY(res->body_len == 0))
    return true;

  res->body = headers_end + 4;
parse_body:
  const uint16_t parsed_body_len = res_len - (res->body - buf); 
  const uint16_t available_len = buf_size - res_len;
  assert(res->body_len <= available_len, STR_LEN_PAIR("Malformed HTTP response: body too long"));

  return (parsed_body_len == res->body_len);
}

static void strlower(char *str, uint16_t len)
{
  __m128i upper_A = _mm_set1_epi8('A');
  __m128i upper_Z = _mm_set1_epi8('Z');
  __m128i diff = _mm_set1_epi8('a' - 'A');

  while (LIKELY(len >= 16))
  {
    __m128i chars = _mm_loadu_si128((__m128i *)(str));
    __m128i mask = _mm_and_si128(_mm_cmpgt_epi8(chars, upper_A), _mm_cmplt_epi8(chars, upper_Z));
    __m128i lower_chars = _mm_add_epi8(chars, _mm_and_si128(mask, diff));
    _mm_storeu_si128((__m128i *)(str), lower_chars);
    str += 16;
    len -= 16;
  }

  while (LIKELY(len--))
  {
    if (UNLIKELY(*str >= 'A' && *str <= 'Z'))
      tolower(*str);
    str++;
  }
}

static void header_map_insert(header_map_t *restrict map, const char *restrict key, const uint16_t key_len, const char *restrict value, const uint16_t value_len)
{
  uint16_t index = (uint16_t)murmurhash3(key, strlen(key), 42) % HEADER_MAP_SIZE;
  uint16_t original_index = index;

  uint8_t i = 1;
  while (UNLIKELY(map->entries[index].key))
  {
    index = (original_index + i * i) % HEADER_MAP_SIZE;
    i++;
  }

  map->entries[index] = (header_entry_t){
    .key = key,
    .value = value,
    .key_len = key_len,
    .value_len = value_len
  };
  map->entries_count++;
}

header_entry_t *header_map_get(header_map_t *restrict map, const char *restrict key, const uint16_t key_len)
{
  uint16_t original_index = (uint16_t)murmurhash3(key, key_len, 42) % HEADER_MAP_SIZE;
  uint16_t index = original_index;

  uint8_t i = 1;
  while (UNLIKELY(map->entries[index].key))
  {
    if (LIKELY(map->entries[index].key_len == key_len && memcmp(map->entries[index].key, key, key_len) == 0))
      return &map->entries[index];
    index = (original_index + i * i) % HEADER_MAP_SIZE;
    i++;
  }

  return NULL;
}