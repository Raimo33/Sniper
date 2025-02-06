/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl_primitives.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 09:58:08 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 12:31:59 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "primitives/ssl_primitives.h"

# include <stdio.h> //TODO remove

COLD static void handle_ssl_error(void);

inline int32_t OPENSSL_init_ssl_p(uint64_t opts, const OPENSSL_INIT_SETTINGS *settings)
{
  if (OPENSSL_init_ssl(opts, settings) == 0)
  {
    printf("DEB OPENSSL_INIT_SSL ERROR\n");
    handle_ssl_error();
  }
  return 0;
}

inline SSL_CTX *SSL_CTX_new_p(const SSL_METHOD *method)
{
  SSL_CTX *ctx = SSL_CTX_new(method);
  if (ctx == NULL)
  {
    printf("DEB SSL_CTX_NEW ERROR\n");
    handle_ssl_error();
  }
  return ctx;
}

inline int32_t SSL_CTX_set_min_proto_version_p(SSL_CTX *ctx, int32_t version)
{
  if (SSL_CTX_set_min_proto_version(ctx, version) == 0)
  {
    printf("DEB SSL_CTX_SET_MIN_PROTO_VERSION ERROR\n");
    handle_ssl_error();
  }
  return 0;
}

inline int32_t SSL_CTX_set_max_proto_version_p(SSL_CTX *ctx, int32_t version)
{
  if (SSL_CTX_set_max_proto_version(ctx, version) == 0)
  {
    printf("DEB SSL_CTX_SET_MAX_PROTO_VERSION ERROR\n");
    handle_ssl_error();
  }
  return 0;
}

inline SSL *SSL_new_p(SSL_CTX *ctx)
{
  SSL *ssl = SSL_new(ctx);
  if (ssl == NULL)
  {
    printf("DEB SSL_NEW ERROR\n");
    handle_ssl_error();
  }
  return ssl;
}

inline int32_t SSL_set_fd_p(SSL *ssl, int32_t fd)
{
  const int32_t ret = SSL_set_fd(ssl, fd);
  if (ret == 0)
  {
    printf("DEB SSL_SET_FD ERROR\n");
    handle_ssl_error();
  }
  return ret;
}

inline int32_t EVP_DigestSignInit_p(EVP_MD_CTX *ctx, EVP_PKEY_CTX **pctx, const EVP_MD *type, ENGINE *e, EVP_PKEY *pkey)
{
  const int32_t ret = EVP_DigestSignInit(ctx, pctx, type, e, pkey);
  if (ret <= 0)
  {
    printf("DEB EVP_DIGESTSIGNINIT ERROR\n");
    handle_ssl_error();
  }
  return ret;
}

inline int32_t EVP_DigestSignUpdate_p(EVP_MD_CTX *ctx, const void *d, size_t cnt)
{
  const int32_t ret = EVP_DigestSignUpdate(ctx, d, cnt);
  if (ret <= 0)
  {
    printf("DEB EVP_DIGESTSIGNUPDATE ERROR\n");
    handle_ssl_error();
  }
  return ret;
}

inline int32_t EVP_DigestSignFinal_p(EVP_MD_CTX *ctx, uint8_t *sig, size_t *siglen)
{
  const int32_t ret = EVP_DigestSignFinal(ctx, sig, siglen);
  if (ret <= 0)
  {
    printf("DEB EVP_DIGESTSIGNFINAL ERROR\n");
    handle_ssl_error();
  }
  return ret;
}

inline int32_t SSL_write_p(SSL *ssl, const void *buf, int32_t num)
{
  const int32_t ret = SSL_write(ssl, buf, num);
  if (ret <= 0)
  {
    const int32_t ssl_error = SSL_get_error(ssl, ret);
    if (ssl_error == SSL_ERROR_WANT_WRITE || ssl_error == SSL_ERROR_WANT_READ)
      return 0;
    printf("DEB SSL_WRITE ERROR\n");
    handle_ssl_error();
  }
  return ret;
}

inline int32_t SSL_read_p(SSL *ssl, void *buf, int32_t num)
{
  const int32_t ret = SSL_read(ssl, buf, num);
  if (ret <= 0)
  {
    const int32_t ssl_error = SSL_get_error(ssl, ret);
    if (ssl_error == SSL_ERROR_WANT_WRITE || ssl_error == SSL_ERROR_WANT_READ)
      return 0;
    printf("DEB SSL_READ ERROR\n");
    handle_ssl_error();
  }
  return ret;
}

inline int32_t SSL_connect_p(SSL *ssl)
{
  const int32_t ret = SSL_connect(ssl);
  if (ret <= 0)
  {
    const int32_t ssl_error = SSL_get_error(ssl, ret);
    if (ssl_error == SSL_ERROR_WANT_WRITE || ssl_error == SSL_ERROR_WANT_READ)
      return 0;
    printf("DEB SSL_CONNECT ERROR\n");
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