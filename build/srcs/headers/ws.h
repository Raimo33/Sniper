/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:53:00 by craimond          #+#    #+#             */
/*   Updated: 2025/01/31 12:35:33 by craimond         ###   ########.fr       */
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
# include "http_parser.h"
# include "dns_resolver.h"
# include "message_broker.h"

# define WS_HOST "data-stream.binance.com"
# define WS_PORT 9443
# define WS_PORT_STR "9443"
# define WS_MAX_PATH_LEN 256
# define WS_FILENO 5
# define WS_READ_BUFFER_SIZE 4096
# define WS_WRITE_BUFFER_SIZE 4096
//TODO double check binance timeouts
# define WS_KEEPALIVE_IDLE 300
# define WS_KEEPALIVE_INTVL 30
# define WS_KEEPALIVE_CNT 3

typedef struct
{
  struct sockaddr_in addr;
  SSL *ssl;
  char path[WS_MAX_PATH_LEN];
  uint8_t conn_key[WS_KEY_SIZE];
  char write_buffer[WS_WRITE_BUFFER_SIZE] ALIGNED(16);
  char read_buffer[WS_READ_BUFFER_SIZE] ALIGNED(16);
  http_response_t http_response;
  uint16_t write_offset;
  uint16_t read_offset;
} ws_client_t;

void COLD init_ws(ws_client_t *restrict client, SSL_CTX *restrict ssl_ctx);
bool HOT handle_ws_connection(ws_client_t *restrict client, const uint8_t events, dns_resolver_t *restrict resolver);
void COLD free_ws(ws_client_t *restrict client);

#endif