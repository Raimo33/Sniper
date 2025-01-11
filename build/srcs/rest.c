/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:53:55 by craimond          #+#    #+#             */
/*   Updated: 2025/01/11 10:27:49 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/rest.h"

//TODO wolfssl?
void init_rest(rest_client_t *const rest)
{
  rest->addr = (struct sockaddr_in){
    .sin_family = AF_INET,
    .sin_port = htons(REST_PORT),
    .sin_addr = {
      .s_addr = inet_addr(REST_HOST) //TODO getaddrinfo e dns resolve
    }
  };

  rest->fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

  setsockopt(rest->fd, IPPROTO_TCP, TCP_NODELAY, &(uint8_t){1}, sizeof(uint8_t));
  setsockopt(rest->fd, SOL_SOCKET, SO_KEEPALIVE, &(uint8_t){1}, sizeof(uint8_t));
  setsockopt(rest->fd, IPPROTO_TCP, TCP_KEEPIDLE, &(uint8_t){REST_KEEPALIVE_IDLE}, sizeof(uint8_t));
  setsockopt(rest->fd, IPPROTO_TCP, TCP_KEEPINTVL, &(uint8_t){REST_KEEPALIVE_INTVL}, sizeof(uint8_t));
  setsockopt(rest->fd, IPPROTO_TCP, TCP_KEEPCNT, &(uint8_t){REST_KEEPALIVE_CNT}, sizeof(uint8_t));

  connect(rest->fd, (const struct sockaddr *)&addr, sizeof(addr));
}

void free_rest(rest_client_t *const rest)
{
  // wolfSSL_free(rest->ssl.ssl);
  // wolfSSL_CTX_free(rest->ssl.ctx);
  close(rest->fd);
}