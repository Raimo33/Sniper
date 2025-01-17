/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 10:10:46 by craimond          #+#    #+#             */
/*   Updated: 2025/01/17 17:35:23 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SSL_H
# define SSL_H

# include <stdint.h>
# include <wolfssl/options.h>
# include <wolfssl/ssl.h>

# include "keys.h"

typedef struct
{
  WOLFSSL_CTX *ctx;
  WOLFSSL *ssl;
} ssl_sock_t;

void __attribute__((cold)) init_ssl(void);
void __attribute__((cold)) init_ssl_socket(const uint16_t fd, ssl_sock_t *ssl);
//TODO void  generate_signature(
void __attribute__((cold)) generate_api_key_signature(const byte *api_key, byte *signature);
void __attribute__((cold)) free_ssl_socket(const ssl_sock_t *ssl);

#endif