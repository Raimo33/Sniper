/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 10:10:46 by craimond          #+#    #+#             */
/*   Updated: 2025/01/27 15:00:15 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SSL_H
# define SSL_H

# include <stdint.h>
# include <openssl/ssl.h>
# include <openssl/crypto.h>
# include <openssl/err.h>
# include <openssl/tls1.h>

# include "extensions.h"
# include "keys.h"

//https://docs.openssl.org/master/man3/

void COLD init_ssl(SSL_CTX **restrict ctx);
SSL *COLD init_ssl_socket(const uint16_t fd, const SSL_CTX *restrict ctx);
//TODO funzioni per generare firme
void COLD free_ssl_socket(SSL *restrict ssl);
void COLD free_ssl(const SSL_CTX *restrict ctx);

#endif