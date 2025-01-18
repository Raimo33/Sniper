/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:53:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/18 10:08:40 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/ws.h"

static void send_upgrade(const ws_client_t *ws);
static void receive_upgrade(const ws_client_t *ws);

//TODO pool di connessioni
void init_ws(ws_client_t *ws)
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
  init_ssl_socket(fd, &ws->ssl_sock);

  dup2(fd, WS_FILENO);
  close(fd);
}

bool handle_ws_connection_event(const ws_client_t *ws)
{
  static uint8_t sequence;

  //TODO altri PREFETCH sequenziali in base a cosa serve allo step successivo
  switch (sequence) //TODO computed gotos (1-2% performance increase)
  {
    case 0:
      connect(WS_FILENO, (struct sockaddr *)&ws->addr, sizeof(ws->addr));
      PREFETCHW(&ws->ssl_sock.ssl, L0);
      sequence++;
      break;
    case 1 ... 5: //TODO stabilire il numero effettivo di step
      wolfSSL_connect(ws->ssl_sock.ssl);
      sequence++;
      break;
    case 6:
      send_upgrade(ws);
      sequence++;
      break;
    case 7: FALLTHROUGH;
      receive_upgrade(ws);
      sequence++;
    default:
      return true;
  }

  return false;
}

void handle_ws_event(const ws_client_t *ws)
{
  //TODO
}

static void send_upgrade(const ws_client_t *ws)
{
  byte ws_key[WS_KEY_SIZE];
  generate_ws_key(ws_key);

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
      { STR_LEN_PAIR("Sec-WebSocket-Key"), ws_key, WS_KEY_SIZE },
    }
    .body = NULL,
    .body_len = 0
  };

  uint16_t req_len = compute_requet_len(&request);
  char raw_request[req_len + 1];
  build_http_request(&request, raw_request);
  wolfSSL_write(ws->ssl_sock.ssl, raw_request, sizeof(raw_request));
}

static void receive_upgrade(const ws_client_t *ws)
{
  //base64 decode, 258EAFA5-E914-47DA-95CA-C5AB0DC85B11, sha1
  //TODO wolfSSL_read, FULL_READ perche siamo in non-blocking ET
}

void free_ws(const ws_client_t *ws)
{
  free_ssl_socket(&ws->ssl_sock);
  close(WS_FILENO);
}