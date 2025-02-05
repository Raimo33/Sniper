/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:01:43 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 21:32:24 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/keys.h"

void init_keys(keys_t *restrict keys)
{
  OpenSSL_add_all_algorithms();

  const char *restrict priv_key = getenv("PRIV_KEY");
  const char *restrict api_key  = getenv("API_KEY");

  fast_assert(priv_key && api_key, "Missing keys");

  memcpy(keys->api_key, api_key, API_KEY_SIZE);
  BIO *bio = BIO_new_mem_buf(priv_key, -1);
  keys->priv_key = PEM_read_bio_PrivateKey(bio, NULL, NULL, NULL);
  fast_assert(keys->priv_key, "Failed to read private key");
  BIO_free(bio);

  fast_assert(EVP_PKEY_get_id(keys->priv_key) == EVP_PKEY_ED25519, "Invalid private key type");
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
  return !memcmp(decoded_key, sha1_hash, sizeof(sha1_hash));
}

void sign_ecd25519(EVP_PKEY *key, const char *data, const uint16_t data_len, char *restrict buffer)
{
  fast_assert(key && data && buffer, "Unexpected NULL pointer");

  EVP_MD_CTX *ctx = EVP_MD_CTX_new();
  EVP_DigestSignInit(ctx, NULL, NULL, NULL, key);

  uint16_t signature_len = ECD25519_SIG_SIZE;
  EVP_DigestSign(ctx, (uint8_t *)buffer, (size_t *)&signature_len, (const uint8_t *)data, data_len);

  EVP_MD_CTX_free(ctx);
}

uint16_t base64_encode(const char *data, const uint16_t data_len, char *restrict buffer, const uint16_t buffer_size)
{
  fast_assert(data && buffer, "Unexpected NULL pointer");
  fast_assert(buffer_size >= BASE64_SIZE(data_len), "Buffer too small");

  return EVP_EncodeBlock((uint8_t *)buffer, (const uint8_t *)data, data_len);
}

void free_keys(keys_t *restrict keys)
{
  if (UNLIKELY(keys == NULL))
    return;

  EVP_PKEY_free(keys->priv_key);
}