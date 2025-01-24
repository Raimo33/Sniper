/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:53:55 by craimond          #+#    #+#             */
/*   Updated: 2025/01/24 16:40:30 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/rest.h"

void init_rest(rest_client_t *restrict rest, const keys_t *restrict keys, const WOLFSSL_CTX *restrict ssl_ctx)
{
  rest->addr = (struct sockaddr_in){
    .sin_family = AF_INET,
    .sin_port = htons(REST_PORT),
    .sin_addr = {
      .s_addr = inet_addr(REST_HOST) //TODO getaddrinfo e dns resolve
    }
  };
  rest->keys = keys;

  const uint16_t fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(uint8_t){1}, sizeof(uint8_t));
  setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &(uint8_t){1}, sizeof(uint8_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &(uint8_t){REST_KEEPALIVE_IDLE}, sizeof(uint8_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &(uint8_t){REST_KEEPALIVE_INTVL}, sizeof(uint8_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &(uint8_t){REST_KEEPALIVE_CNT}, sizeof(uint8_t));
  rest->ssl = init_ssl_socket(fd, ssl_ctx);

  dup2(fd, REST_FILENO);
  close(fd);
}

inline bool handle_rest_connection(const rest_client_t *restrict rest, const char fd_state)
{
  static void *restrict states[] = { &&connect, &&ssl_handshake };
  static uint8_t sequence;

  if (UNLIKELY(fd_state == 'e'))
    panic(STR_LEN_PAIR("REST connection error"));

  goto *states[sequence];

connect:
  connect(REST_FILENO, &rest->addr, sizeof(rest->addr));
  sequence++;
  return false;

ssl_handshake:
  sequence += wolfSSL_connect(rest->ssl) == SSL_SUCCESS;
  return true;
}

void free_rest(const rest_client_t *restrict rest)
{
  free_ssl_socket(rest->ssl);
  close(REST_FILENO);
}