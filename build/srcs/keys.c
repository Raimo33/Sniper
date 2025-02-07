/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:01:43 by craimond          #+#    #+#             */
/*   Updated: 2025/02/07 16:41:37 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "keys.h"

void init_keys(keys_t *restrict keys)
{
  const char *priv_key = getenv("PRIV_KEY");
  const char *api_key = getenv("API_KEY");

  fast_assert(priv_key && api_key, "Missing environment variables");

  memcpy(keys->api_key, api_key, API_KEY_SIZE);
  keys->priv_key = EVP_PKEY_new_raw_private_key_p(EVP_PKEY_ED25519, NULL, (const uint8_t *)priv_key, PRIV_KEY_SIZE);
}

void generate_ws_key(uint8_t *restrict key)
{
  fast_assert(key, "Unexpected NULL pointer");

  uint8_t random_data[16];
  RAND_bytes(random_data, sizeof(random_data));

  const uint8_t encoded_size = EVP_EncodeBlock(key, random_data, sizeof(random_data));
  fast_assert(encoded_size == WS_KEY_SIZE, "Failed to encode ws key");
}

bool verify_ws_key(const uint8_t *restrict key, const uint8_t *restrict accept, const uint16_t len)
{
  fast_assert(key && accept, "Unexpected NULL pointer");

  uint8_t decoded_key[WS_KEY_SIZE];

  const uint8_t decoded_size = EVP_DecodeBlock(decoded_key, accept, len);
  fast_assert(decoded_size == WS_KEY_SIZE, "Failed to decode ws key");

  uint8_t concatenated_key[WS_KEY_SIZE + STR_LEN(WS_KEY_GUID)];
  memcpy(concatenated_key, key, WS_KEY_SIZE);
  memcpy(concatenated_key + WS_KEY_SIZE, STR_AND_LEN(WS_KEY_GUID));

  uint8_t sha1_hash[20];
  SHA1(concatenated_key, sizeof(concatenated_key), sha1_hash);
  return (memcmp(decoded_key, sha1_hash, sizeof(sha1_hash)) == 0);
}

void sign_ed25519(EVP_PKEY *key, const char *data, const uint16_t data_len, char *restrict buffer)
{
  fast_assert(key && data && buffer, "Unexpected NULL pointer");

  EVP_MD_CTX *ctx = EVP_MD_CTX_new();
  EVP_DigestSignInit_p(ctx, NULL, NULL, NULL, key);

  size_t signature_len = ED25519_SIG_SIZE;
  EVP_DigestSign_p(ctx, (unsigned char *)buffer, &signature_len, (const unsigned char *)data, data_len);

  EVP_MD_CTX_free(ctx);
}

uint16_t base64_encode(const uint8_t *data, const uint16_t data_len, uint8_t *restrict buffer, const uint16_t buffer_size)
{
  fast_assert(data && buffer, "Unexpected NULL pointer");
  fast_assert(buffer_size >= BASE64_ENCODED_SIZE(data_len), "Buffer too small for encoding");

  return EVP_EncodeBlock(buffer, data, data_len);
}

uint16_t base64_decode(const uint8_t *data, const uint16_t data_len, uint8_t *restrict buffer, const uint16_t buffer_size)
{
  fast_assert(data && buffer, "Unexpected NULL pointer");
  fast_assert(buffer_size >= BASE64_DECODED_SIZE(data_len), "Buffer too small for decoding");

  return EVP_DecodeBlock(buffer, data, data_len);
}

void free_keys(keys_t *restrict keys)
{
  if (UNLIKELY(keys == NULL))
    return;

  EVP_PKEY_free(keys->priv_key);
}