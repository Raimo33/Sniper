/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 10:10:46 by craimond          #+#    #+#             */
/*   Updated: 2025/01/19 19:23:17 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SSL_H
# define SSL_H

# include <stdint.h>
# include <wolfssl/options.h>
# include <wolfssl/ssl.h>

# include "extensions.h"
# include "keys.h"

# define CYPHER_SUITE "TLS_AES_128_GCM_SHA256"

void COLD init_ssl(WOLFSSL_CTX **restrict ctx);
SSL *COLD init_ssl_socket(const uint16_t fd, const WOLFSSL_CTX *restrict ctx);
//TODO funzioni per generare firme
void COLD free_ssl_socket(SSL *restrict ssl);
void COLD free_ssl(const WOLFSSL_CTX *restrict ctx);

#endif