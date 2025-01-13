/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:53:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/13 21:15:13 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/ws.h"

static void perform_ws_handshake(const WOLFSSL *const ssl, WC_RNG *const rng);

//TODO pool di connessioni
void init_ws(ws_client_t *const ws, ssl_data_t *ssl_data)
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

  connect(fd, (const struct sockaddr *)&ws->addr, sizeof(ws->addr));
  wolfSSL_connect(ws->ssl_sock.ssl);
  perform_ws_handshake(ws->ssl_sock.ssl, &ssl_data->rng);

  dup2(fd, WS_FILENO);
  close(fd);
}

//https://developers.binance.com/docs/binance-spot-api-docs/web-socket-streams#trade-streams
static void perform_ws_handshake(const WOLFSSL *const ssl, WC_RNG *const rng) {

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
    generate_ws_key(rng, buffer + request_len);

    wolfSSL_write(ssl, buffer, sizeof(buffer));
}

static void construct_path()

void free_ws(ws_client_t *const ws)
{
  free_ssl_socket(&ws->ssl_sock);
  close(WS_FILENO);
}