/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:53:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/27 19:34:46 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/ws.h"

static bool COLD send_upgrade_request(ws_client_t *restrict ws);
static bool COLD receive_upgrade_response(const ws_client_t *restrict ws);

//TODO pool di connessioni
void init_ws(ws_client_t *restrict ws, const SSL_CTX *restrict ssl_ctx)
{
  ws->addr = (struct sockaddr_in){
    .sin_family = AF_INET,
    .sin_port = htons(WS_PORT),
    .sin_addr = {
      .s_addr = INADDR_NONE
    }
  };
  
  const uint16_t fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt(fd, IPPROTO_TCP, TCP_FASTOPEN, &(uint8_t){1}, sizeof(uint8_t));
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(uint8_t){1}, sizeof(uint8_t));
  ws->ssl = init_ssl_socket(fd, ssl_ctx);

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

static bool send_upgrade_request(ws_client_t *restrict ws)
{
  static http_request_t request ALIGNED(16) =
  {
    .method = HTTP_GET,
    .path = WS_PATH,
    .path_len = STR_LEN(WS_PATH),
    .version = HTTP_1_1,
    .headers_count = 5,
    .headers = {
      { STR_LEN_PAIR("Host"), STR_LEN_PAIR(WS_HOST) },
      { STR_LEN_PAIR("Upgrade"), STR_LEN_PAIR("websocket") },
      { STR_LEN_PAIR("Connection"), STR_LEN_PAIR("Upgrade") },
      { STR_LEN_PAIR("Sec-WebSocket-Version"), STR_LEN_PAIR("13") },
      { STR_LEN_PAIR("Sec-WebSocket-Key"), NULL, WS_KEY_SIZE }
    },
    .body = NULL,
    .body_len = 0
  };
  static const uint16_t len = (
    STR_LEN("GET") + 1 + STR_LEN(WS_PATH) + 1 + STR_LEN("HTTP/1.1") + 2 +
    STR_LEN("Host") + 2 + STR_LEN(WS_HOST) + 2 +
    STR_LEN("Upgrade") + 2 + STR_LEN("websocket") + 2 +
    STR_LEN("Connection") + 2 + STR_LEN("Upgrade") + 2 +
    STR_LEN("Sec-WebSocket-Version") + 2 + STR_LEN("13") + 2 +
    STR_LEN("Sec-WebSocket-Key") + 2 + WS_KEY_SIZE + 4
  );

  if (UNLIKELY(request.headers[4].value == NULL))
  {
    generate_ws_key(ws->conn_key);
    request.headers[4].value = ws->conn_key;
    build_http_request(&request, ws->write_buffer);
  }

  const uint16_t bytes_sent = SSL_write(ws->ssl, ws->write_buffer, len);
  memset(ws->write_buffer, 0, len);
  return bytes_sent == len;
}

static bool receive_upgrade_response(const ws_client_t *restrict ws)
{
  const uint16_t len = SSL_read(ws->ssl, ws->read_buffer, WS_READ_BUFFER_SIZE);
  
  static http_response_t response ALIGNED(16);
  if (LIKELY(parse_http_response(ws->read_buffer, WS_READ_BUFFER_SIZE, &response, len) == false))
    return false;
  
  fast_assert(response.status_code == 101, STR_LEN_PAIR("Websocket upgrade failed: invalid status code"));
  fast_assert(response.headers.entries_count == 3, STR_LEN_PAIR("Websocket upgrade failed: missing response headers"));

  const header_entry_t *restrict accept_header = header_map_get(&response.headers, STR_LEN_PAIR("Sec-WebSocket-Accept"));
  fast_assert(accept_header, STR_LEN_PAIR("Websocket upgrade failed: missing Upgrade header"));

  if (verify_ws_key(ws->conn_key, accept_header->value, accept_header->value_len) == false)
    panic(STR_LEN_PAIR("Websocket upgrade failed: key mismatch"));

  memset(ws->read_buffer, 0, len);
  return false;
}

void free_ws(const ws_client_t *restrict ws)
{
  free_ssl_socket(ws->ssl);
  close(WS_FILENO);
}