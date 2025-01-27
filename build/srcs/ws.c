/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:53:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/27 14:04:56 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/ws.h"

static bool COLD send_upgrade_request(const ws_client_t *restrict ws);
static bool COLD receive_upgrade_response(const ws_client_t *restrict ws);

//TODO pool di connessioni
void init_ws(ws_client_t *restrict ws, const WOLFSSL_CTX *restrict ssl_ctx)
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
  static void *restrict states[] = {
  static uint8_t sequence = 0;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic(STR_LEN_PAIR("Websocket connection error"));

  goto *states[sequence];

dns_query:
  log_msg(STR_LEN_PAIR("Resolving Websocket endpoint: " WS_HOST));
  resolve_domain(resolver, STR_LEN_PAIR(WS_HOST), &client->addr, WS_FILENO);
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
  sequence += wolfSSL_connect(client->ssl) == SSL_SUCCESS;
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
  static bool init;
  static const http_request_t request ALIGNED(16) =
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
      { STR_LEN_PAIR("Sec-WebSocket-Key"), ws->conn_key, WS_KEY_SIZE }
    }
    .body = NULL,
    .body_len = 0
  };
  static const uint16_t len = (
    STR_LEN("GET") + 1 + request.path_len + 1 + STR_LEN("HTTP/1.1") + 2 +
    request.headers[0].key_len + 2 + request.headers[0].value_len + 2 +
    request.headers[1].key_len + 2 + request.headers[1].value_len + 2 +
    request.headers[2].key_len + 2 + request.headers[2].value_len + 2 +
    request.headers[3].key_len + 2 + request.headers[3].value_len + 2 +
    request.headers[4].key_len + 2 + request.headers[4].value_len + 2 + 2
  );
  static char buffer[len] ALIGNED(16);

  if (UNLIKELY(init == false))
  {
    generate_ws_key(ws->conn_key);
    build_http_request(&request, buffer);
    init = true;
  }

  return !!wolfSSL_send(ws->ssl, buffer, len, MSG_NOSIGNAL);
}

static bool receive_upgrade_response(const ws_client_t *restrict ws)
{
  static char buffer[512] ALIGNED(16);
  static header_t headers[8] ALIGNED(16);
  static http_response_t response ALIGNED(16) = {
    .headers = headers,
    .headers_count = sizeof(headers) / sizeof(header_t)
  };

  const uint16_t len = wolfSSL_recv(ws->ssl_sock.ssl, buffer, sizeof(buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
  if (len <= 0)
    return false;
  
  parse_http_response(buffer, len, &response);
  
  assert(response.status_code == 101, STR_LEN_PAIR("Websocket upgrade failed: invalid status code"));
  assert(response.headers_count == 3, STR_LEN_PAIR("Websocket upgrade failed: missing response headers"));

  const header_entry_t *restrict accept_header = header_map_get(&response.headers, STR_LEN_PAIR("Sec-WebSocket-Accept"));
  assert(key, STR_LEN_PAIR("Websocket upgrade failed: missing Upgrade header"));

  if (verify_ws_key(ws->conn_key, accept_header->value, accept_header->value_len) == false)
    panic(STR_LEN_PAIR("Websocket upgrade failed: key mismatch"));

  return false;
}

void free_ws(const ws_client_t *restrict ws)
{
  free_ssl_socket(ws->ssl);
  close(WS_FILENO);
}