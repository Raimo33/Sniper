/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:53:00 by craimond          #+#    #+#             */
/*   Updated: 2025/01/25 14:19:10 by craimond         ###   ########.fr       */
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
# include "ssl.h"
# include "keys.h"
# include "http_parser.h"
# include "shared_buffers.h"

# define WS_HOST "data-stream.binance.com"
# define WS_PORT 9443
# define WS_PORT_STR "9443"
# define WS_FILENO 5
# define WS_PATH "/stream?timeUnit=microsecond?streams=usdcusdt@trade/usdpusdt@trade/usdtdai@trade/fdusdusdt@trade/fdusdusdc@trade/tusdusdt@trade/eureuri@trade"

typedef struct
{
  struct sockaddr_in addr;
  SSL *ssl;
  uint8_t conn_key[WS_KEY_SIZE] ALIGNED(16);
} ws_client_t;

void COLD init_ws(ws_client_t *restrict ws, const WOLFSSL_CTX *restrict ssl_ctx);
inline bool HOT handle_ws_connection(const ws_client_t *restrict ws, const uint8_t events);
void COLD free_ws(const ws_client_t *restrict ws);

#endif