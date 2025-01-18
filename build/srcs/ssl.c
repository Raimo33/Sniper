/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:35:17 by craimond          #+#    #+#             */
/*   Updated: 2025/01/18 18:16:33 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "headers/ssl.h"

void init_ssl(void)
{
  wolfSSL_Init();
}

void init_ssl_socket(const uint16_t fd, ssl_sock_t *ssl_sock)
{
  ssl_sock->ctx = wolfSSL_CTX_new(wolfTLSv1_3_client_method());
  ssl_sock->ssl = wolfSSL_new(ssl_sock->ctx);

  wolfSSL_set_fd(ssl_sock->ssl, fd);
  wolfSSL_set_verify(ssl_sock->ssl, SSL_VERIFY_NONE, NULL);
  wolfSSL_set_using_nonblock(ssl_sock->ssl, true);
}

//TODO generate per-message signature (0.03ms max)
// void generate_signature(
// {
  
// }

void free_ssl_socket(const ssl_sock_t *ssl_sock)
{
  wolfSSL_shutdown(ssl_sock->ssl);
  wolfSSL_free(ssl_sock->ssl);
  wolfSSL_CTX_free(ssl_sock->ctx);
}

void free_ssl(void)
{
  wolfSSL_Cleanup();
}