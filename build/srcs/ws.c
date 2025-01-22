/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:53:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/22 19:59:48 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/ws.h"

static bool send_upgrade_request(const ws_client_t *restrict ws);
static bool receive_upgrade_response(const ws_client_t *restrict ws);

//TODO pool di connessioni
void init_ws(ws_client_t *restrict ws, const WOLFSSL_CTX *restrict ssl_ctx)
{
  ws->addr = (struct sockaddr_in){
    .sin_family = AF_INET,
    .sin_port = htons(WS_PORT),
    .sin_addr = {
      .s_addr = inet_addr(WS_HOST) //TODO getaddrinfo e dns resolve (https://c-ares.org/docs.html)
    }
  };
  
  const uint16_t fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(uint8_t){1}, sizeof(uint8_t));
  ws->ssl = init_ssl_socket(fd, ssl_ctx);

  dup2(fd, WS_FILENO);
  close(fd);
}

inline bool handle_ws_connection(const ws_client_t *restrict ws, const char fd_state)
{
  static void *restrict states[] = { &&connect, &&ssl_handshake, &&upgrade_request, &&upgrade_response };
  static uint8_t sequence;

  if (UNLIKELY(fd_state == 'e'))
    panic(STR_LEN_PAIR("Websocket connection error"));

  goto *states[sequence];

connect:
  connect(WS_FILENO, &ws->addr, sizeof(ws->addr));
  sequence++;
  return false;

ssl_handshake:
  if (wolfSSL_connect(ws->ssl_sock.ssl) == SSL_SUCCESS)
    sequence++;
  return false;

upgrade_request:
  if (send_upgrade_request(ws))
    sequence++;
  return false;

upgrade_response:
  if (receive_upgrade_response(ws))
    sequence++;
  return true;
}

static bool send_upgrade_request(const ws_client_t *restrict ws)
{
  static byte ws_key[WS_KEY_SIZE] ALIGNED(16);
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
      { STR_LEN_PAIR("Sec-WebSocket-Key"), ws_key, WS_KEY_SIZE }
    }
    .body = NULL,
    .body_len = 0
  };
  static uint16_t len = 
    STR_LEN("GET") + 1 + request.path_len + 1 + STR_LEN("HTTP/1.1") + 2 +
    request.headers[0].key_len + 2 + request.headers[0].value_len + 2 +
    request.headers[1].key_len + 2 + request.headers[1].value_len + 2 +
    request.headers[2].key_len + 2 + request.headers[2].value_len + 2 +
    request.headers[3].key_len + 2 + request.headers[3].value_len + 2 +
    request.headers[4].key_len + 2 + request.headers[4].value_len + 2 + 2;
  static char buffer[len] ALIGNED(16);

  if (LIKELY(!ws_key[0]))
  {
    generate_ws_key(ws_key);
    build_http_request(&request, buffer);
  }

  return !!wolfSSL_send(ws->ssl_sock.ssl, buffer, len, MSG_NOSIGNAL);
}

static bool receive_upgrade_response(const ws_client_t *restrict ws)
{
  static char buffer[512] ALIGNED(16);
  static header_t headers[8] ALIGNED(16);
  static http_response_t response ALIGNED(16) = {
    .headers = headers,
    .headers_count = sizeof(headers) / sizeof(header_t)
  };

  if (UNLIKELY(wolfssl_recv(ws->ssl_sock.ssl, buffer, sizeof(buffer), MSG_NOSIGNAL | MSG_DONTWAIT) <= 0))
    return false;

  parse_http_response(buffer, sizeof(buffer), &response);
  
  //TODO base64 decode, 258EAFA5-E914-47DA-95CA-C5AB0DC85B11, sha1
  //controllo dello status code 101, controllo dell'header Sec-WebSocket-Accept etc
}

void free_ws(const ws_client_t *restrict ws)
{
  free_ssl_socket(ws->ssl);
  close(WS_FILENO);
}

/*

HTTP/1.1 200 OK\r\n
Content-Type: text/html; charset=UTF-8\r\n
Content-Length: 138\r\n
Connection: keep-alive\r\n
\r\n
body


*/