/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:01:43 by craimond          #+#    #+#             */
/*   Updated: 2025/01/23 16:28:24 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/keys.h"

static WC_RNG rng;

void init_keys(keys_t *restrict keys)
{
  wolfCrypt_Init();
  wc_InitRng(&rng);

  const byte *restrict priv_key = getenv("PRIV_KEY");
  const byte *restrict api_key  = getenv("API_KEY");

  assert(priv_key && api_key, STR_LEN_PAIR("Missing keys"));

  memcpy(keys->api_key, api_key, API_KEY_SIZE);
  wc_ed25519_import_private_only(priv_key, ED25519_PRIV_KEY_SIZE, keys->priv_key);
}

void generate_ws_key(byte *restrict key)
{
  byte random_data[16];
  wc_RNG_GenerateBlock(&rng, random_data, sizeof(random_data));

  word32 encoded_size = WS_KEY_SIZE;
  Base64_Encode(random_data, sizeof(random_data), key, &encoded_size);

  assert(encoded_size == WS_KEY_SIZE, STR_LEN_PAIR("Failed to encode ws key"));
}

void verify_ws_key(const byte *restrict key, const char *restrict accept, const uint16_t len)
{
  byte decoded_key[WS_KEY_SIZE];
  word32 decoded_size = WS_KEY_SIZE;
  Base64_Decode(accept, len, decoded_key, &decoded_size);
  assert(decoded_size == WS_KEY_SIZE, STR_LEN_PAIR("Failed to decode ws key"));

  byte concatenated_key[WS_KEY_SIZE + STR_LEN(WS_KEY_GUID)];
  memcpy(concatenated_key, key, WS_KEY_SIZE);
  memcpy(concatenated_key + WS_KEY_SIZE, STR_LEN_PAIR(WS_KEY_GUID));

  byte sha1_hash[20];
  wc_Sha1Hash(concatenated_key, sizeof(concatenated_key), sha1_hash);
  assert(memcmp(decoded_key, sha1_hash, sizeof(sha1_hash)) == 0, STR_LEN_PAIR("Invalid ws key"));
}

void free_keys(keys_t *restrict keys)
{
  wc_ed25519_free(keys->priv_key);
  wc_FreeRng(&rng);
  wolfCrypt_Cleanup();
}