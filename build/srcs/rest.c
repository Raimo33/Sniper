/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:53:55 by craimond          #+#    #+#             */
/*   Updated: 2025/01/15 19:01:57 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/rest.h"

void init_rest(rest_client_t *rest, const keys_t *keys)
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
  init_ssl_socket(fd, &rest->ssl_sock);

  dup2(fd, REST_FILENO);
  close(fd);
}

bool handle_rest_connection_event(const rest_client_t *rest, const uint32_t events)
{
  static uint8_t sequence;

  switch (sequence)
  {
    case 0:
      connect(REST_FILENO, (struct sockaddr *)&rest->addr, sizeof(rest->addr));
      sequence++;
      break;
    case 1:
      if (wolfSSL_connect(rest->ssl_sock.ssl) == SSL_SUCCESS)
        sequence++;
      break;
    default:
      break;
  }

  return (sequence >= 2);
}

void handle_rest_event(const rest_client_t *rest)
{
  //TODO
}

void free_rest(const rest_client_t *rest)
{
  free_ssl_socket(&rest->ssl_sock);
  close(REST_FILENO);
}