/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:53:00 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 13:20:55 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WS_H
# define WS_H

# include <stdint.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <netdb.h>

# include "extensions.h"
# include "fast_ssl.h"
# include "keys.h"
# include "http_serializer.h"
# include "dns_resolver.h"
# include "message_broker.h"
# include "graph.h"

# define WS_HOST "data-stream.binance.com"
# define WS_PORT 9443
# define WS_PORT_STR "9443"
# define WS_PATH "/stream?timeUnit=microsecond"
# define WS_FILENO 5
# define WS_READ_BUFFER_SIZE 4096
# define WS_WRITE_BUFFER_SIZE 4096
# define WS_KEEPALIVE_IDLE 300
# define WS_KEEPALIVE_INTVL 30
# define WS_KEEPALIVE_CNT 3

typedef struct
{
  struct sockaddr_in addr;
  SSL *ssl;
  uint8_t conn_key[WS_KEY_SIZE];
  char *write_buffer;
  char *read_buffer;
  http_response_t http_response;
  uint32_t write_offset;
  uint32_t read_offset;
  bool connected;
} ws_client_t;

COLD void init_ws(ws_client_t *restrict client, SSL_CTX *restrict ssl_ctx);
HOT void handle_ws_connection(ws_client_t *restrict client, const uint32_t events, dns_resolver_t *restrict resolver);
HOT void handle_ws_setup(ws_client_t *restrict client, const uint32_t events, graph_t *restrict graph);
HOT void handle_ws_trading(ws_client_t *restrict client, const uint32_t events, graph_t *restrict graph);
COLD void free_ws(ws_client_t *restrict client);

#endif