/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:53:34 by craimond          #+#    #+#             */
/*   Updated: 2025/02/03 22:52:17 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/ws.h"

COLD static bool send_upgrade_query(ws_client_t *restrict client);
COLD static bool receive_upgrade_response(ws_client_t *restrict client);

//TODO pool di connessioni
void init_ws(ws_client_t *restrict client, SSL_CTX *restrict ssl_ctx)
{
  const uint16_t fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt(fd, IPPROTO_TCP, TCP_FASTOPEN, &(bool){true}, sizeof(bool));
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(bool){true}, sizeof(bool));
  setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &(bool){true}, sizeof(bool));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &(uint16_t){WS_KEEPALIVE_IDLE}, sizeof(uint16_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &(uint16_t){WS_KEEPALIVE_INTVL}, sizeof(uint16_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &(uint16_t){WS_KEEPALIVE_CNT}, sizeof(uint16_t));

  *client = (ws_client_t){
    .addr = {
      .sin_family = AF_INET,
      .sin_port = htons(WS_PORT),
      .sin_addr = {
        .s_addr = INADDR_NONE
      }
    },
    .ssl = init_ssl_socket(fd, ssl_ctx),
    .conn_key = {},
    .write_buffer = calloc(WS_WRITE_BUFFER_SIZE, sizeof(char)),
    .read_buffer = calloc(WS_READ_BUFFER_SIZE, sizeof(char)),
    .http_response = {},
    .write_offset = 0,
    .read_offset = 0
  };

  dup2(fd, WS_FILENO);
  close(fd);
}

void handle_ws_connection(ws_client_t *restrict client, const uint8_t events, dns_resolver_t *restrict resolver)
{
  static void *restrict states[] = {&&dns_query, &&dns_response, &&connect, &&ssl_handshake, &&upgrade_query, &&upgrade_response};
  static uint8_t sequence = 0;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic(STR_AND_LEN("Websocket connection error"));

  goto *states[sequence];

dns_query:
  log_msg(STR_AND_LEN("Resolving Websocket endpoint: " WS_HOST));
  resolve_domain(resolver, STR_AND_LEN(WS_HOST), WS_FILENO);
  sequence++;
  return;

dns_response:
  log_msg(STR_AND_LEN("Resolved Websocket endpoint: " WS_HOST));
  read(WS_FILENO, &client->addr.sin_addr.s_addr, sizeof(client->addr.sin_addr.s_addr));
  sequence++;
  return;

connect:
  log_msg(STR_AND_LEN("Connecting to Websocket endpoint: " WS_HOST));
  connect(WS_FILENO, (struct sockaddr *)&client->addr, sizeof(client->addr));
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
  client->connected = receive_upgrade_response(client);
}

void handle_ws_setup(ws_client_t *restrict client, const uint8_t events, graph_t *restrict graph)
{
  static void *restrict states[] = {};
  static uint8_t sequence = 0;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic(STR_AND_LEN("Websocket setup error"));

  goto *states[sequence];

  (void)graph;
  (void)client;
  (void)events;
  //TODO subscribe to the streams once the graph is formed (derive path from graph)
}

void handle_ws_trading(ws_client_t *restrict client, const uint8_t events, graph_t *restrict graph)
{
  static void *restrict states[] = {};
  static uint8_t sequence = 0;

  goto *states[sequence];

  (void)graph;
  (void)client;
  (void)events;
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
        { STR_AND_LEN("Host"), STR_AND_LEN(WS_HOST ":" WS_PORT_STR) },
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
    panic(STR_AND_LEN("Websocket upgrade failed: key mismatch"));

  free_http_response(&client->http_response);
  return true;
}

void free_ws(ws_client_t *restrict client)
{
  close(WS_FILENO);

  if (UNLIKELY(client == NULL))
    return;

  free(client->write_buffer);
  free(client->read_buffer);
  free_ssl_socket(client->ssl);
}