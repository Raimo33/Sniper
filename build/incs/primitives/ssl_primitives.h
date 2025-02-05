/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl_primitives.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:06:06 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 12:49:56 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SSL_PRIMITIVES_H
# define SSL_PRIMITIVES_H

# include <stdint.h>
# include <openssl/ssl.h>
# include <openssl/err.h>

# include "extensions.h"
# include "errors.h"

COLD extern inline int32_t OPENSSL_init_ssl_p(uint64_t opts, const OPENSSL_INIT_SETTINGS *settings);
COLD extern inline SSL_CTX *SSL_CTX_new_p(const SSL_METHOD *method);
COLD extern inline int32_t SSL_CTX_set_min_proto_version_p(SSL_CTX *ctx, int32_t version);
COLD extern inline int32_t SSL_CTX_set_max_proto_version_p(SSL_CTX *ctx, int32_t version);
COLD extern inline SSL *SSL_new_p(SSL_CTX *ctx);
COLD extern inline int32_t SSL_set_fd_p(SSL *ssl, int fd);
HOT extern inline int32_t EVP_DigestSignInit_p(EVP_MD_CTX *ctx, EVP_PKEY_CTX **pctx, const EVP_MD *type, ENGINE *e, EVP_PKEY *pkey);
HOT extern inline int32_t EVP_DigestSign_p(EVP_MD_CTX *ctx, unsigned char *sigret, size_t *siglen, const unsigned char *tbs, size_t tbslen);
HOT extern inline int32_t SSL_write_p(SSL *ssl, const void *buf, int num);
HOT extern inline int32_t SSL_read_p(SSL *ssl, void *buf, int num);
COLD extern inline int32_t SSL_connect_p(SSL *ssl);

#endif