/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fast_ssl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 10:10:46 by craimond          #+#    #+#             */
/*   Updated: 2025/01/31 10:25:57 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FAST_SSL_H
# define FAST_SSL_H

# include <stdint.h>
# include <openssl/ssl.h>
# include <openssl/crypto.h>
# include <openssl/err.h>
# include <openssl/tls1.h>

# include "primitives/ssl_primitives.h"
# include "extensions.h"
# include "keys.h"

# define TLS_CIPHERSUITE "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256"
# define TLS_CIPHER_LIST "TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256:TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA:TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384:TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA"

//https://docs.openssl.org/master/man3/

COLD SSL_CTX *init_ssl(void);
COLD SSL *init_ssl_socket(const uint8_t fd, SSL_CTX *restrict ctx, const char *restrict host);
COLD void free_ssl_socket(SSL *restrict ssl);
COLD void free_ssl(SSL_CTX *restrict ctx);

#endif