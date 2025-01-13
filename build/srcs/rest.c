/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:53:55 by craimond          #+#    #+#             */
/*   Updated: 2025/01/13 19:43:27 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/rest.h"

//TODO add wolfssl ed25519 + api key
void init_rest(rest_client_t *const rest, ssl_data_t *ssl_data)
{
  rest->addr = (struct sockaddr_in){
    .sin_family = AF_INET,
    .sin_port = htons(REST_PORT),
    .sin_addr = {
      .s_addr = inet_addr(REST_HOST) //TODO getaddrinfo e dns resolve
    }
  };

  const uint16_t fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(uint8_t){1}, sizeof(uint8_t));
  setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &(uint8_t){1}, sizeof(uint8_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &(uint8_t){REST_KEEPALIVE_IDLE}, sizeof(uint8_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &(uint8_t){REST_KEEPALIVE_INTVL}, sizeof(uint8_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &(uint8_t){REST_KEEPALIVE_CNT}, sizeof(uint8_t));
  init_ssl_socket(fd, &rest->ssl_sock);

  connect(fd, (const struct sockaddr *)&rest->addr, sizeof(rest->addr));
  wolfSSL_connect(rest->ssl_sock.ssl);

  dup2(fd, REST_FILENO);
  close(fd);
}

void free_rest(rest_client_t *const rest)
{
  free_ssl_socket(&rest->ssl_sock);
  close(REST_FILENO);
}