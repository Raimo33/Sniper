/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:53:34 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 10:28:03 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clients/ws.h"

COLD static bool send_upgrade_query(ws_client_t *restrict client);
COLD static bool receive_upgrade_response(ws_client_t *restrict client);

void init_ws(ws_client_t *restrict client, SSL_CTX *restrict ssl_ctx)
{
  const uint8_t fd = socket_p(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt_p(fd, IPPROTO_TCP, TCP_FASTOPEN, &(int32_t){5}, sizeof(int32_t));
  setsockopt_p(fd, IPPROTO_TCP, TCP_NODELAY, &(int32_t){1}, sizeof(int32_t));
  setsockopt_p(fd, SOL_SOCKET, SO_KEEPALIVE, &(int32_t){1}, sizeof(int32_t));
  setsockopt_p(fd, IPPROTO_TCP, TCP_KEEPIDLE, &(int32_t){WS_KEEPALIVE_IDLE}, sizeof(int32_t));
  setsockopt_p(fd, IPPROTO_TCP, TCP_KEEPINTVL, &(int32_t){WS_KEEPALIVE_INTVL}, sizeof(int32_t));
  setsockopt_p(fd, IPPROTO_TCP, TCP_KEEPCNT, &(int32_t){WS_KEEPALIVE_CNT}, sizeof(int32_t));

  *client = (ws_client_t){
    .sock_fd = fd,
    .addr = {},
    .ssl = init_ssl_socket(fd, ssl_ctx),
    .conn_key = {},
    .write_buffer = calloc_p(WS_WRITE_BUFFER_SIZE, sizeof(char)),
    .read_buffer = calloc_p(WS_READ_BUFFER_SIZE, sizeof(char)),
    .http_response = {},
    .write_offset = 0,
    .read_offset = 0
  };
}

void handle_ws_connection(const uint8_t fd, const uint32_t events, void *data)
{
  static void *restrict states[] = {&&connect, &&ssl_handshake, &&upgrade_query, &&upgrade_response};
  static uint8_t sequence = 0;

  ws_client_t *client = data;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic("Websocket connection error"); //TODO continuare a mettere wrappers

  goto *states[sequence];

connect:
  log_msg(STR_AND_LEN("Connecting to Websocket endpoint: " WS_HOST));
  connect(fd, (struct sockaddr *)&client->addr, sizeof(client->addr));
  sequence++;
  return;

ssl_handshake:
  log_msg(STR_AND_LEN("Performing SSL handshake"));
  sequence += SSL_connect(client->ssl) == true;
  return;

upgrade_query:
  log_msg(STR_AND_LEN("Sending Websocket upgrade query"));
  sequence += send_upgrade_query(client);
  return;

upgrade_response:
  log_msg(STR_AND_LEN("Receiving Websocket upgrade response"));
  client->status = receive_upgrade_response(client) ? CONNECTED : DISCONNECTED;
}

void handle_ws_setup(const uint8_t fd, const uint32_t events, void *data)
{
  static void *restrict states[] = {};
  static uint8_t sequence = 0;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic("Websocket connection error");

  goto *states[sequence];

  (void)fd;
  (void)data;

  //TODO subscribe to the streams once the graph is formed (derive path from graph)
  // client->status = TRADING;
}

void handle_ws_trading(const uint8_t fd, const uint32_t events, void *data)
{
  static void *restrict states[] = {};
  static uint8_t sequence = 0;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic("Websocket connection error");

  goto *states[sequence];

  (void)fd;
  (void)data;

  //TODO receive price data and update graph
}

static bool send_upgrade_query(ws_client_t *restrict client)
{
  static bool initialized;
  static uint16_t len;

  if (!initialized)
  {
    generate_ws_key(client->conn_key);
    http_request_t request = {
      .method = GET,
      .path = WS_PATH,
      .path_len = STR_LEN(WS_PATH),
      .version = HTTP_1_1,
      .headers = (header_entry_t[]){
        { STR_AND_LEN("Host"), STR_AND_LEN(WS_HOST ":" WS_PORT) },
        { STR_AND_LEN("Upgrade"), STR_AND_LEN("websocket") },
        { STR_AND_LEN("Connection"), STR_AND_LEN("Upgrade") },
        { STR_AND_LEN("Sec-WebSocket-Key"), (char *)client->conn_key, WS_KEY_SIZE }
      },
      .n_headers = 4,
      .body = NULL,
      .body_len = 0
    };
    len = serialize_http_request(client->write_buffer, WS_WRITE_BUFFER_SIZE, &request);
    initialized = true;
  }
  
  return try_ssl_send(client->ssl, client->write_buffer, len, &client->write_offset);
}

static bool receive_upgrade_response(ws_client_t *restrict client)
{
  if (UNLIKELY(try_ssl_recv_http(client->ssl, client->read_buffer, WS_READ_BUFFER_SIZE, &client->read_offset, &client->http_response) == false))
    return false;

  const http_response_t *restrict response = &client->http_response;
  fast_assert(response->status_code == 101, "Websocket upgrade failed: invalid status code");
  fast_assert(response->headers.n_entries == 3, "Websocket upgrade failed: missing response headers");

  const header_entry_t *accept_header = header_map_get(&response->headers, STR_AND_LEN("Sec-WebSocket-Accept"));
  fast_assert(accept_header, "Websocket upgrade failed: missing Upgrade header");

  if (verify_ws_key(client->conn_key, (uint8_t *)accept_header->value, accept_header->value_len) == false)
    panic("Websocket upgrade failed: key mismatch");

  free_http_response(&client->http_response);
  return true;
}

void free_ws(ws_client_t *restrict client)
{
  close(client->sock_fd);

  if (UNLIKELY(client == NULL))
    return;

  free(client->write_buffer);
  free(client->read_buffer);
  free_ssl_socket(client->ssl);
}