/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:53:00 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 21:54:12 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WS_H
# define WS_H

# include <stdint.h>

# include "wrappers.h"
# include "client_common.h"
# include "extensions.h"
# include "keys.h"
# include "graph.h"

# define WS_HOST "stream.binance.com"
# define WS_PORT "9443"
# define WS_PATH "/stream?timeUnit=microsecond"
# define WS_READ_BUFFER_SIZE 4096
# define WS_WRITE_BUFFER_SIZE 4096
# define WS_KEEPALIVE_IDLE 300
# define WS_KEEPALIVE_INTVL 30
# define WS_KEEPALIVE_CNT 3

typedef struct
{
  uint16_t sock_fd;
  struct sockaddr_in addr;
  SSL *ssl;
  uint8_t conn_key[WS_KEY_SIZE];
  char *write_buffer;
  char *read_buffer;
  http_response_t http_response;
  uint32_t write_offset;
  uint32_t read_offset;
  client_status_t status;
} ws_client_t;

COLD void init_ws(ws_client_t *restrict client, SSL_CTX *restrict ssl_ctx);
HOT void handle_ws_connection(const uint8_t fd, const uint32_t events, void *data);
HOT void handle_ws_setup(const uint8_t fd, const uint32_t events, void *data);
HOT void handle_ws_trading(const uint8_t fd, const uint32_t events, void *data);
COLD void free_ws(ws_client_t *restrict client);

#endif