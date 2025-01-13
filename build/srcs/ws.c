/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:53:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/13 19:17:25 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/ws.h"

//TODO pool di connessioni
void init_ws(ws_client_t *const ws)
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
  perform_ws_handshake(ws->ssl_sock.ssl);

  dup2(fd, WS_FILENO);
  close(fd);
}

static void perform_ws_handshake(const WOLFSSL *const ssl)
{
  //TODO dynamically form path, dynamically generate Sec-WebSocket-Key (in keys.c)
  const char *const path = "/stream?streams=usdcusdt@trade/usdpusdt@trade/usdtdai@trade/fdusdusdt@trade/fdusdusdc@trade/tusdusdt@trade/eureuri@trade";
  char ws_key[WS_KEY_SIZE];

  //TODO generate_ws_key(&ws->rng, ws_key);
  //https://developers.binance.com/docs/binance-spot-api-docs/web-socket-streams#trade-streams
  const char *const request[] = 
    "GET %s HTTP/1.1\r\n"
    "Host: " WS_HOST ":%d\r\n"
    "Upgrade: websocket\r\n"
    "Connection: Upgrade\r\n"
    "Sec-WebSocket-Key: %s\r\n"
    "Sec-WebSocket-Version: 13\r\n"
    "\r\n";

  //TODO sprintf?? (qualcosa di meglio?)

  wolfSSL_write(ssl, request, sizeof(request) - 1);
}

void free_ws(ws_client_t *const ws)
{
  free_ssl_socket(&ws->ssl_sock);
  close(WS_FILENO);
}