/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fast_ssl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:35:17 by craimond          #+#    #+#             */
/*   Updated: 2025/02/07 18:21:38 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fast_ssl.h"

SSL_CTX *init_ssl(void)
{  
  OPENSSL_init_ssl_p(0, NULL);

  SSL_CTX *ctx = SSL_CTX_new_p(TLS_client_method());

  SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);
  SSL_CTX_set_min_proto_version_p(ctx, TLS1_2_VERSION);
  SSL_CTX_set_max_proto_version_p(ctx, TLS1_3_VERSION);
  SSL_CTX_set_cipher_list_p(ctx, TLS_CIPHER_LIST);
  SSL_CTX_set_ciphersuites_p(ctx, TLS_CIPHERSUITE);

  SSL_CTX_set_read_ahead(ctx, true);
  SSL_CTX_set_mode(ctx, SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER | SSL_MODE_ENABLE_PARTIAL_WRITE | SSL_MODE_AUTO_RETRY | SSL_MODE_RELEASE_BUFFERS);

  return ctx;
}

SSL *init_ssl_socket(const uint8_t fd, SSL_CTX *restrict ctx, const char *restrict host)
{
  SSL *ssl = SSL_new_p(ctx);
  SSL_set_fd_p(ssl, fd);
  SSL_set_tlsext_host_name(ssl, host);
  return ssl;
}

void free_ssl_socket(SSL *restrict ssl)
{
  if (UNLIKELY(ssl == NULL))
    return;

  SSL_free(ssl);
}

void free_ssl(SSL_CTX *restrict ctx)
{
  SSL_CTX_free(ctx);
  OPENSSL_cleanup();
}