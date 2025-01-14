/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 10:10:46 by craimond          #+#    #+#             */
/*   Updated: 2025/01/14 19:31:14 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SSL_H
# define SSL_H

# include <stdint.h>
# include <wolfssl/options.h>
# include <wolfssl/ssl.h>

typedef struct
{
  WOLFSSL_CTX *ctx;
  WOLFSSL *ssl;
} ssl_sock_t;

void  init_ssl(void);
void  init_ssl_socket(const uint16_t fd, ssl_sock_t *ssl);
//TODO void  generate_signature(
void  free_ssl_socket(const ssl_sock_t *ssl);

#endif