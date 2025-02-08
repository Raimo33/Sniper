/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crypto_primitives.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 13:12:51 by craimond          #+#    #+#             */
/*   Updated: 2025/02/08 13:13:53 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "primitives/crypto_primitives.h"

# include <stdio.h> //TODO remove

COLD static void handle_ssl_error(void);

inline EVP_PKEY *EVP_PKEY_new_raw_private_key_p(int type, ENGINE *e, const uint8_t *key, size_t keylen)
{
  EVP_PKEY *pkey = EVP_PKEY_new_raw_private_key(type, e, key, keylen);
  if (UNLIKELY(pkey == NULL))
  {
    printf("DEB EVP_PKEY_NEW_RAW_PRIVATE_KEY ERROR\n");
    handle_ssl_error();
  }
  return pkey;
}

inline int32_t EVP_DigestSignInit_p(EVP_MD_CTX *ctx, EVP_PKEY_CTX **pctx, const EVP_MD *type, ENGINE *e, EVP_PKEY *pkey)
{
  const int32_t ret = EVP_DigestSignInit(ctx, pctx, type, e, pkey);
  if (UNLIKELY(ret <= 0))
  {
    printf("DEB EVP_DIGESTSIGNINIT ERROR\n");
    handle_ssl_error();
  }
  return ret;
}

inline int32_t EVP_DigestSign_p(EVP_MD_CTX *ctx, uint8_t *sigret, size_t *siglen, const uint8_t *tbs, size_t tbslen)
{
  const int32_t ret = EVP_DigestSign(ctx, sigret, siglen, tbs, tbslen);
  if (UNLIKELY(ret <= 0))
  {
    printf("DEB EVP_DIGESTSIGN ERROR\n");
    handle_ssl_error();
  }
  return ret;
}

inline int32_t RAND_bytes_p(uint8_t *buf, int32_t num)
{
  const int32_t ret = RAND_bytes(buf, num);
  if (UNLIKELY(ret == 0))
  {
    printf("DEB RAND_BYTES ERROR\n");
    handle_ssl_error();
  }
  return ret;
}

inline int32_t EVP_EncodeBlock_p(uint8_t *t, const uint8_t *f, int n)
{
  const int32_t ret = EVP_EncodeBlock(t, f, n);
  if (UNLIKELY(ret == 0))
  {
    printf("DEB EVP_ENCODEBLOCK ERROR\n");
    handle_ssl_error();
  }
  return ret;
}

inline int32_t EVP_DecodeBlock_p(uint8_t *t, const uint8_t *f, int n)
{
  const int32_t ret = EVP_DecodeBlock(t, f, n);
  if (UNLIKELY(ret == 0))
  {
    printf("DEB EVP_DECODEBLOCK ERROR\n");
    handle_ssl_error();
  }
  return ret;
}


static void handle_ssl_error(void)
{
  char msg[256] = {0};
  ERR_error_string_n(ERR_get_error(), msg, sizeof(msg));
  panic(msg);
}