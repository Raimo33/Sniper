/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:01:43 by craimond          #+#    #+#             */
/*   Updated: 2025/02/08 13:12:27 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "keys.h"

void init_keys(keys_t *restrict keys)
{
  const char *priv_key = getenv("PRIV_KEY");
  const char *api_key = getenv("API_KEY");

  fast_assert(priv_key && api_key, "Missing environment variables");

  memcpy(keys->api_key, api_key, API_KEY_SIZE);
  keys->priv_key = EVP_PKEY_new_raw_private_key_p(EVP_PKEY_ED25519, NULL, (const uint8_t *)priv_key, ED25519_KEYLEN);
}

void sign_ed25519(EVP_PKEY *key, const char *data, const uint16_t data_len, char *restrict buffer)
{
  fast_assert(key && data && buffer, "Unexpected NULL pointer");

  EVP_MD_CTX *ctx = EVP_MD_CTX_new();
  EVP_DigestSignInit_p(ctx, NULL, NULL, NULL, key);

  size_t signature_len = ED25519_SIGSIZE;
  EVP_DigestSign_p(ctx, (uint8_t *)buffer, &signature_len, (const uint8_t *)data, data_len);

  EVP_MD_CTX_free(ctx);
}

void generate_ws_key(char *restrict buffer, const uint8_t buffer_size)
{
  fast_assert(buffer, "Unexpected NULL pointer");
  fast_assert(buffer_size >= BASE64_ENCODED_SIZE(WS_KEY_SIZE) + 1, "Buffer too small for encoding");

  uint8_t rand_bytes[WS_KEY_SIZE];

  RAND_bytes_p(rand_bytes, sizeof(rand_bytes));
  EVP_EncodeBlock_p((uint8_t *)buffer, rand_bytes, sizeof(rand_bytes));
}

bool verify_ws_key(const uint8_t *restrict key, const uint8_t key_len, const uint8_t *restrict accept, const uint16_t accept_len)
{
  fast_assert(key && accept, "Unexpected NULL pointer");
  fast_assert(key_len == BASE64_ENCODED_SIZE(WS_KEY_SIZE), "Invalid ws key size");
  fast_assert(accept_len == BASE64_ENCODED_SIZE(SHA1_DIGEST_SIZE), "Invalid accept key size");

  uint8_t concat[BASE64_ENCODED_SIZE(WS_KEY_SIZE) + STR_LEN(WS_MAGIC_GUID)];
  memcpy(concat, key, key_len);
  memcpy(concat + key_len, STR_AND_LEN(WS_MAGIC_GUID));

  uint8_t hash[SHA_DIGEST_LENGTH];
  SHA1(concat, sizeof(concat), hash);

  uint8_t computed_accept[BASE64_ENCODED_SIZE(SHA_DIGEST_LENGTH) + 1] = {0};
  const uint8_t computed_len = EVP_EncodeBlock(computed_accept, hash, SHA_DIGEST_LENGTH);
  fast_assert(computed_len == accept_len, "Invalid accept key size");

  return !CRYPTO_memcmp(computed_accept, accept, accept_len);
}

void free_keys(keys_t *restrict keys)
{
  if (UNLIKELY(keys == NULL))
    return;

  EVP_PKEY_free(keys->priv_key);
}