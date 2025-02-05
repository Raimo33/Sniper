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

# include "extensions.h"
# include "keys.h"

//https://docs.openssl.org/master/man3/

COLD void init_ssl(SSL_CTX **restrict ctx);
COLD SSL *init_ssl_socket(const uint16_t fd, SSL_CTX *restrict ctx);
COLD void free_ssl_socket(SSL *restrict ssl);
COLD void free_ssl(SSL_CTX *restrict ctx);

#endif