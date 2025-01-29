/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:53:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/29 20:59:05 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/ws.h"

static bool COLD send_upgrade_request(ws_client_t *restrict client);
static bool COLD receive_upgrade_response(const ws_client_t *restrict client);

//TODO pool di connessioni
void init_ws(ws_client_t *restrict client, const SSL_CTX *restrict ssl_ctx)
{
  client->addr = (struct sockaddr_in){
    .sin_family = AF_INET,
    .sin_port = htons(WS_PORT),
    .sin_addr = {
      .s_addr = INADDR_NONE
    }
  };
  
  const uint16_t fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt(fd, IPPROTO_TCP, TCP_FASTOPEN, &(bool){true}, sizeof(bool));
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(bool){true}, sizeof(bool));
  setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &(bool){true}, sizeof(bool));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &(uint8_t){WS_KEEPALIVE_IDLE}, sizeof(uint8_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &(uint8_t){WS_KEEPALIVE_INTVL}, sizeof(uint8_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &(uint8_t){WS_KEEPALIVE_CNT}, sizeof(uint8_t));
  client->ssl = init_ssl_socket(fd, ssl_ctx);

  dup2(fd, WS_FILENO);
  close(fd);
}

inline bool handle_ws_connection(const ws_client_t *restrict client, const uint8_t events, const dns_resolver_t *restrict resolver)
{
  static void *restrict states[] = {&&dns_query, &&dns_response, &&connect, &&ssl_handshake, &&upgrade_request, &&upgrade_response};
  static uint8_t sequence = 0;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic(STR_LEN_PAIR("Websocket connection error"));

  goto *states[sequence];

dns_query:
  log_msg(STR_LEN_PAIR("Resolving Websocket endpoint: " WS_HOST));
  resolve_domain(resolver, STR_LEN_PAIR(WS_HOST), WS_FILENO);
  sequence++;
  return false;

dns_response:
  log_msg(STR_LEN_PAIR("Resolved Websocket endpoint: " WS_HOST));
  read(WS_FILENO, &client->addr.sin_addr.s_addr, sizeof(client->addr.sin_addr.s_addr));
  sequence++;
  return false;

connect:
  log_msg(STR_LEN_PAIR("Connecting to Websocket endpoint: " WS_HOST));
  connect(WS_FILENO, &client->addr, sizeof(client->addr));
  sequence++;
  return false;

ssl_handshake:
  log_msg(STR_LEN_PAIR("Performing SSL handshake"));
  sequence += SSL_connect(client->ssl) == true;
  return false;

upgrade_request:
  log_msg(STR_LEN_PAIR("Sending Websocket upgrade request"));
  sequence += send_upgrade_request(client);
  return false;

upgrade_response:
  log_msg(STR_LEN_PAIR("Receiving Websocket upgrade response"));
  return receive_upgrade_response(client);
}

static bool send_upgrade_request(ws_client_t *restrict client)
{
  static const char first_part[] = 
    "GET " WS_PATH " HTTP/1.1\r\n"
    "Host: " WS_HOST ":" WS_PORT_STR "\r\n"
    "Upgrade: websocket\r\n"
    "Connection: Upgrade\r\n"
    "Sec-WebSocket-Version: 13\r\n"
    "Sec-WebSocket-Key: ";
  static const char second_part[] = "\r\n\r\n";
  static const uint16_t len = sizeof(first_part) + WS_KEY_SIZE + sizeof(second_part) - 1;

  generate_ws_key(client->conn_key); //TODO call only once
  //TODO merge the two parts in a single buffer

  return try_ssl_send(client->ssl, client->write_buffer, len, &client->write_offset);
}

static bool receive_upgrade_response(const ws_client_t *restrict client)
{
  if (UNLIKELY(try_ssl_recv_http(client->ssl, client->read_buffer, WS_READ_BUFFER_SIZE, &client->read_offset, &client->http_response) == false))
    return false;

  //TODO check Connection: closed header

  const http_response_t *response = &client->http_response;
  fast_assert(response->status_code == 101, STR_LEN_PAIR("Websocket upgrade failed: invalid status code"));
  fast_assert(response->headers.entries_count == 3, STR_LEN_PAIR("Websocket upgrade failed: missing response headers"));

  const header_entry_t *accept_header = header_map_get(&response->headers, STR_LEN_PAIR("Sec-WebSocket-Accept"));
  fast_assert(accept_header, STR_LEN_PAIR("Websocket upgrade failed: missing Upgrade header"));

  if (verify_ws_key(client->conn_key, accept_header->value, accept_header->value_len) == false)
    panic(STR_LEN_PAIR("Websocket upgrade failed: key mismatch"));

  return false;
}

void free_ws(const ws_client_t *restrict client)
{
  free_ssl_socket(client->ssl);
  close(WS_FILENO);
}