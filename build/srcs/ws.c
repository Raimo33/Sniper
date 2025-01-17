/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:53:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/17 20:43:23 by craimond         ###   ########.fr       */
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
  static const http_request_t request = //TODO
  const char request[] ALIGNED(16) =
    "GET " WS_PATH " HTTP/1.1"
    "\r\nHost: " WS_HOST ":" WS_PORT_STR
    "\r\nUpgrade: websocket"
    "\r\nConnection: Upgrade"
    "\r\nSec-WebSocket-Version: 13"
    "\r\nSec-WebSocket-Key: ";

  char buffer[STR_LEN(request) + WS_KEY_SIZE] ALIGNED(16);

  memcpy(buffer, request, STR_LEN(request));
  generate_ws_key(buffer + STR_LEN(request));

  wolfSSL_write(ws->ssl_sock.ssl, buffer, sizeof(buffer));
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