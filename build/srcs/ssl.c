/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:35:17 by craimond          #+#    #+#             */
/*   Updated: 2025/01/26 17:21:32 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//https://www.wolfssl.com/documentation/manuals/wolfssl

# include "headers/ssl.h"

void init_ssl(WOLFSSL_CTX **restrict ctx)
{
  wolfSSL_Init();
  *ctx = wolfSSL_CTX_new(wolfSSL_v3_client_method());
  wolfSSL_CTX_set_verify(*ctx, SSL_VERIFY_NONE, NULL);
  wolfSSL_CTX_set_cihper_list(*ctx, CYPHER_SUITE);
  wolfSSL_dtls13_use_quick_timeout(*ctx, 1);
  wolfSSL_CTX_set_read_ahead(*ctx, 1);
}

SSL *init_ssl_socket(const uint16_t fd, const WOLFSSL_CTX *restrict ctx)
{
  SSL *ssl = wolfSSL_new(ctx);

  wolfSSL_set_fd(ssl, fd);
  wolfSSL_set_verify(ssl, SSL_VERIFY_NONE, NULL);
  wolfSSL_set_using_nonblock(ssl, true);

  return ssl;
}

//TODO generate per-message signature (0.03ms max)
// void generate_signature(
// {
  
// }

void free_ssl_socket(SSL *restrict ssl)
{
  wolfSSL_shutdown(ssl);
  wolfSSL_free(ssl);
}

void free_ssl(const WOLFSSL_CTX *restrict ctx)
{
  wolfSSL_CTX_free(ctx);
  wolfSSL_Cleanup();
}