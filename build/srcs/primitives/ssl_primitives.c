/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl_primitives.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 09:58:08 by craimond          #+#    #+#             */
/*   Updated: 2025/02/07 18:50:21 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "primitives/ssl_primitives.h"

# include <stdio.h> //TODO remove

COLD static void handle_ssl_error(void);

inline int32_t OPENSSL_init_ssl_p(uint64_t opts, const OPENSSL_INIT_SETTINGS *settings)
{
  if (UNLIKELY(OPENSSL_init_ssl(opts, settings) == 0))
  {
    printf("DEB OPENSSL_INIT_SSL ERROR\n");
    handle_ssl_error();
  }
  return 0;
}

inline SSL_CTX *SSL_CTX_new_p(const SSL_METHOD *method)
{
  SSL_CTX *ctx = SSL_CTX_new(method);
  if (UNLIKELY(ctx == NULL))
  {
    printf("DEB SSL_CTX_NEW ERROR\n");
    handle_ssl_error();
  }
  return ctx;
}

inline int32_t SSL_CTX_set_min_proto_version_p(SSL_CTX *ctx, int32_t version)
{
  if (UNLIKELY(SSL_CTX_set_min_proto_version(ctx, version) == 0))
  {
    printf("DEB SSL_CTX_SET_MIN_PROTO_VERSION ERROR\n");
    handle_ssl_error();
  }
  return 0;
}

inline int32_t SSL_CTX_set_max_proto_version_p(SSL_CTX *ctx, int32_t version)
{
  if (UNLIKELY(SSL_CTX_set_max_proto_version(ctx, version) == 0))
  {
    printf("DEB SSL_CTX_SET_MAX_PROTO_VERSION ERROR\n");
    handle_ssl_error();
  }
  return 0;
}

inline int32_t SSL_CTX_set_ciphersuites_p(SSL_CTX *ctx, const char *str)
{
  if (UNLIKELY(SSL_CTX_set_ciphersuites(ctx, str) == 0))
  {
    printf("DEB SSL_CTX_SET_CIPHERSUITES ERROR\n");
    handle_ssl_error();
  }
  return 0;
}

inline int32_t SSL_CTX_set_cipher_list_p(SSL_CTX *ctx, const char *str)
{
  if (UNLIKELY(SSL_CTX_set_cipher_list(ctx, str) == 0))
  {
    printf("DEB SSL_CTX_SET_CIPHER_LIST ERROR\n");
    handle_ssl_error();
  }
  return 0;
}

inline SSL *SSL_new_p(SSL_CTX *ctx)
{
  SSL *ssl = SSL_new(ctx);
  if (UNLIKELY(ssl == NULL))
  {
    printf("DEB SSL_NEW ERROR\n");
    handle_ssl_error();
  }
  return ssl;
}

inline int32_t SSL_set_fd_p(SSL *ssl, int32_t fd)
{
  const int32_t ret = SSL_set_fd(ssl, fd);
  if (UNLIKELY(ret == 0))
  {
    printf("DEB SSL_SET_FD ERROR\n");
    handle_ssl_error();
  }
  return ret;
}

inline EVP_PKEY *EVP_PKEY_new_raw_private_key_p(int type, ENGINE *e, const unsigned char *key, size_t keylen)
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

inline int32_t EVP_DigestSign_p(EVP_MD_CTX *ctx, unsigned char *sigret, size_t *siglen, const unsigned char *tbs, size_t tbslen)
{
  const int32_t ret = EVP_DigestSign(ctx, sigret, siglen, tbs, tbslen);
  if (UNLIKELY(ret <= 0))
  {
    printf("DEB EVP_DIGESTSIGN ERROR\n");
    handle_ssl_error();
  }
  return ret;
}

inline int32_t SSL_write_p(SSL *ssl, const void *buf, int32_t num)
{
  const int32_t ret = SSL_write(ssl, buf, num);
  if (LIKELY(ret <= 0))
  {
    const int32_t ssl_error = SSL_get_error(ssl, ret);
    if (LIKELY(ssl_error == SSL_ERROR_WANT_WRITE || ssl_error == SSL_ERROR_WANT_READ))
      return 0;
    printf("DEB SSL_WRITE ERROR\n");
    handle_ssl_error();
  }
  return ret;
}

inline int32_t SSL_read_p(SSL *ssl, void *buf, int32_t num)
{
  const int32_t ret = SSL_read(ssl, buf, num);
  if (LIKELY(ret <= 0))
  {
    const int32_t ssl_error = SSL_get_error(ssl, ret);
    if (LIKELY(ssl_error == SSL_ERROR_WANT_WRITE || ssl_error == SSL_ERROR_WANT_READ))
      return 0;
    printf("DEB SSL_READ ERROR\n");
    handle_ssl_error();
  }
  return ret;
}

inline int32_t SSL_connect_p(SSL *ssl)
{
  const int32_t ret = SSL_connect(ssl);
  if (LIKELY(ret <= 0))
  {
    const int32_t ssl_error = SSL_get_error(ssl, ret);
    if (LIKELY(ssl_error == SSL_ERROR_WANT_WRITE || ssl_error == SSL_ERROR_WANT_READ))
      return 0;
    printf("DEB SSL_connect ERROR\n");
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