/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:01:43 by craimond          #+#    #+#             */
/*   Updated: 2025/01/23 20:41:30 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/keys.h"

static WC_RNG rng;

void init_keys(keys_t *restrict keys)
{
  wolfCrypt_Init();
  wc_InitRng(&rng);

  const uint8_t *restrict priv_key = getenv("PRIV_KEY");
  const uint8_t *restrict api_key  = getenv("API_KEY");

  assert(priv_key && api_key, STR_LEN_PAIR("Missing keys"));

  memcpy(keys->api_key, api_key, API_KEY_SIZE);
  wc_ed25519_import_private_only(priv_key, ED25519_PRIV_KEY_SIZE, keys->priv_key);
}

void generate_ws_key(uint8_t *restrict key)
{
  uint8_t random_data[16];
  wc_RNG_GenerateBlock(&rng, random_data, sizeof(random_data));

  word32 encoded_size = WS_KEY_SIZE;
  Base64_Encode(random_data, sizeof(random_data), key, &encoded_size);

  assert(encoded_size == WS_KEY_SIZE, STR_LEN_PAIR("Failed to encode ws key"));
}

bool verify_ws_key(const uint8_t *restrict key, const char *restrict accept, const uint16_t len)
{
  uint8_t decoded_key[WS_KEY_SIZE];
  word32 decoded_size = WS_KEY_SIZE;
  Base64_Decode(accept, len, decoded_key, &decoded_size);
  assert(decoded_size == WS_KEY_SIZE, STR_LEN_PAIR("Failed to decode ws key"));

  uint8_t concatenated_key[WS_KEY_SIZE + STR_LEN(WS_KEY_GUID)];
  memcpy(concatenated_key, key, WS_KEY_SIZE);
  memcpy(concatenated_key + WS_KEY_SIZE, STR_LEN_PAIR(WS_KEY_GUID));

  uint8_t sha1_hash[20];
  wc_Sha1Hash(concatenated_key, sizeof(concatenated_key), sha1_hash); //TODO deprecated
  return !memcmp(decoded_key, sha1_hash, sizeof(sha1_hash))
}

uint32_t murmurhash3(const uint8_t *key, const uint16_t len, const uint32_t seed)
{
  uint32_t h = seed;
  if (len > 3)
  {
    const uint32_t *key_x4 = (const uint32_t *) key;
    size_t i = len >> 2;
    do {
      uint32_t k = *key_x4++;
      k *= 0xcc9e2d51;
      k = (k << 15) | (k >> 17);
      k *= 0x1b873593;
      h ^= k;
      h = (h << 13) | (h >> 19);
      h = (h * 5) + 0xe6546b64;
    } while (--i);
    key = (const uint8_t *) key_x4;
  }
  if (len & 3)
  {
    size_t i = len & 3;
    uint32_t k = 0;
    key = &key[i - 1];
    do {
        k <<= 8;
        k |= *key--;
    } while (--i);
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    h ^= k;
  }
  h ^= len;
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;
  return h;
}

void free_keys(keys_t *restrict keys)
{
  wc_ed25519_free(keys->priv_key);
  wc_FreeRng(&rng);
  wolfCrypt_Cleanup();
}