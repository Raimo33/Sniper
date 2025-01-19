/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 10:10:46 by craimond          #+#    #+#             */
/*   Updated: 2025/01/19 09:55:45 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SSL_H
# define SSL_H

# include <stdint.h>
# include <wolfssl/options.h>
# include <wolfssl/ssl.h>

# include "extensions.h"
# include "keys.h"

typedef struct
{
  WOLFSSL_CTX *ctx;
  WOLFSSL *ssl;
} ssl_sock_t;

void COLD init_ssl(void);
void COLD init_ssl_socket(const uint16_t fd, ssl_sock_t *restrict ssl);
//TODO funzioni per generare firme
void COLD free_ssl_socket(const ssl_sock_t *restrict ssl);

#endif