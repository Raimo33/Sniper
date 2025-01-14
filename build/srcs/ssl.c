/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:35:17 by craimond          #+#    #+#             */
/*   Updated: 2025/01/14 14:47:19 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "headers/ssl.h"

void init_ssl(ssl_data_t *const ssl_data)
{
  wolfSSL_Init();
  wolfCrypt_Init();
  wc_InitRng(&ssl_data->rng);
  init_keys(&ssl_data->keys, &ssl_data->rng);
}

void init_ssl_socket(const uint16_t fd, ssl_sock_t *const ssl_sock)
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

void free_ssl_socket(ssl_sock_t *const ssl_sock)
{
  wolfSSL_free(ssl_sock->ssl);
  wolfSSL_CTX_free(ssl_sock->ctx);
}

void free_ssl(ssl_data_t *const ssl_data)
{
  free_keys(&ssl_data->keys);
  wc_FreeRng(&ssl_data->rng);
  wolfSSL_Cleanup();
  wolfCrypt_Cleanup();
}