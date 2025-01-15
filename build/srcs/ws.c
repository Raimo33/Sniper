/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:53:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/15 18:58:08 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/ws.h"

static void send_ws_upgrade(const ws_client_t *ws);

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

bool handle_ws_connection_event(const ws_client_t *ws, const uint32_t events)
{
  static uint8_t sequence;

  switch (sequence)
  {
    case 0:
      connect(WS_FILENO, (struct sockaddr *)&ws->addr, sizeof(ws->addr));
      sequence++;
      break;
    case 1:
      if (wolfSSL_connect(ws->ssl_sock.ssl) == SSL_SUCCESS)
        sequence++;
      break;
    case 2:
      if (events & EPOLLOUT)
        send_ws_upgrade(ws);
      else
        receive_ws_upgrade(ws);
      sequence++;
      break;
    default:
      break;
  }

  return (sequence >= 3);
}

void handle_ws_event(const ws_client_t *ws)
{
  //TODO
}

static void send_ws_upgrade(const ws_client_t *ws)
{
  const char request[] __attribute__ ((aligned(16))) =
    "GET " WS_PATH " HTTP/1.1"
    "\r\nHost: " WS_HOST ":" WS_PORT_STR
    "\r\nUpgrade: websocket"
    "\r\nConnection: Upgrade"
    "\r\nSec-WebSocket-Version: 13"
    "\r\nSec-WebSocket-Key: ";

  const uint16_t request_len = sizeof(request) - 1;
  char buffer[request_len + WS_KEY_SIZE] __attribute__ ((aligned(16)));

  memcpy(buffer, request, request_len);
  generate_ws_key(buffer + request_len);

  wolfSSL_write(ws->ssl_sock.ssl, buffer, sizeof(buffer));
}

static void receive_ws_upgrade(const ws_client_t *ws)
{
  //TODO wolfSSL_read, FULL_READ perche siamo in non-blocking ET
}

void free_ws(const ws_client_t *ws)
{
  free_ssl_socket(&ws->ssl_sock);
  close(WS_FILENO);
}