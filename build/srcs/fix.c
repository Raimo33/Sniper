/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 21:02:36 by craimond          #+#    #+#             */
/*   Updated: 2025/01/11 10:27:00 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/fix.h"
#include "headers/config.h"

//TODO pool di connessioni
void init_fix(fix_client_t *const fix)
{
  fix->addr = (struct sockaddr_in){
    .sin_family = AF_INET,
    .sin_port = htons(FIX_PORT),
    .sin_addr = {
      .s_addr = inet_addr(FIX_HOST) //TODO getaddrinfo e dns resolve
    }
  };

  fix->fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt(fix->fd, IPPROTO_TCP, TCP_NODELAY, &(uint8_t){1}, sizeof(uint8_t));

  const WOLFSSL_CTX *const ctx = wolfSSL_CTX_new(wolfTLSv1_3_client_method());
  const WOLFSSL *const ssl = wolfSSL_new(ctx);
  wolfSSL_set_fd(ssl, fix->fd);
  
  connect(fix->fd, (const struct sockaddr *)&addr, sizeof(addr));
}

void free_fix(fix_client_t *const fix)
{
  wolfSSL_free(fix->ssl.ssl);
  wolfSSL_CTX_free(fix->ssl.ctx);
  close(fix->fd);
}