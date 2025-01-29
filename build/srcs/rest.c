/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:53:55 by craimond          #+#    #+#             */
/*   Updated: 2025/01/29 21:00:40 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/rest.h"

void init_rest(rest_client_t *restrict client, const keys_t *restrict keys, const SSL_CTX *restrict ssl_ctx)
{
  client->addr = (struct sockaddr_in){
    .sin_family = AF_INET,
    .sin_port = htons(REST_PORT),
    .sin_addr = {
      .s_addr = INADDR_NONE
    }
  };
  client->keys = keys;

  const uint16_t fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt(fd, IPPROTO_TCP, TCP_FASTOPEN, &(bool){true}, sizeof(bool));
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(bool){true}, sizeof(bool));
  setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &(bool){true}, sizeof(bool));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &(uint8_t){REST_KEEPALIVE_IDLE}, sizeof(uint8_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &(uint8_t){REST_KEEPALIVE_INTVL}, sizeof(uint8_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &(uint8_t){REST_KEEPALIVE_CNT}, sizeof(uint8_t));
  client->ssl = init_ssl_socket(fd, ssl_ctx);

  dup2(fd, REST_FILENO);
  close(fd);
}

inline bool handle_rest_connection(const rest_client_t *restrict rest, const uint8_t events, const dns_resolver_t *restrict resolver)
{
  static void *restrict states[] = {&&dns_query, &&dns_response, &&connect, &&ssl_handshake};
  static uint8_t sequence = 0;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic(STR_LEN_PAIR("REST connection error"));

  goto *states[sequence];

dns_query:
  log_msg(STR_LEN_PAIR("Resolving REST endpoint: " REST_HOST));
  resolve_domain(resolver, STR_LEN_PAIR(REST_HOST), REST_FILENO);
  sequence++;
  return false;

dns_response:
  log_msg(STR_LEN_PAIR("Resolved REST endpoint: " REST_HOST));
  read(REST_FILENO, &rest->addr.sin_addr.s_addr, sizeof(rest->addr.sin_addr.s_addr));
  sequence++;
  return false;

connect:
  log_msg(STR_LEN_PAIR("Connecting to REST endpoint: " REST_HOST));
  connect(REST_FILENO, &rest->addr, sizeof(rest->addr));
  sequence++;
  return false;

ssl_handshake:
  log_msg(STR_LEN_PAIR("Performing SSL handshake"));
  return SSL_connect(rest->ssl) == true;

//TODO keepalive nelle richieste successive come header
//TODO fare le query di initializzazione qui?
}

void free_rest(const rest_client_t *restrict rest)
{
  free_ssl_socket(rest->ssl);
  close(REST_FILENO);
}