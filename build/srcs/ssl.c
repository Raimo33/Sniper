/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:35:17 by craimond          #+#    #+#             */
/*   Updated: 2025/01/11 19:03:46 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "headers/ssl.h"

void init_ssl(keys_t *const keys)
{
  wolfSSL_Init();
  generate_keys(keys);
}

ssl_t init_ssl_socket(const uint16_t fd)
{
  const ssl_t ssl = {
    .ctx = wolfSSL_CTX_new(wolfTLSv1_3_client_method()),
    .ssl = wolfSSL_new(ssl.ctx)
  };

  wolfSSL_set_fd(ssl.ssl, fd);
  wolfSSL_set_verify(ssl.ssl, SSL_VERIFY_NONE, NULL);
  wolfSSL_set_using_nonblock(ssl.ssl, true);

  return ssl;
}

//TODO generate per-message signature (0.03ms max)
// void generate_signature(
// {
  
// }

void cleanup_ssl(ssl_t *const ssl)
{
  wolfSSL_free(ssl->ssl);
  wolfSSL_CTX_free(ssl->ctx);
  //wolfSSL_shutdown(ssl);??? //TODO
}