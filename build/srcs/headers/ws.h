/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:53:00 by craimond          #+#    #+#             */
/*   Updated: 2025/01/18 10:00:07 by craimond         ###   ########.fr       */
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

# define WS_HOST "data-stream.binance.com"
# define WS_PORT 9443U
# define WS_PORT_STR "9443"
# define WS_FILENO 5U
# define WS_PATH "/stream?timeUnit=microsecond?streams=usdcusdt@trade/usdpusdt@trade/usdtdai@trade/fdusdusdt@trade/fdusdusdc@trade/tusdusdt@trade/eureuri@trade"

typedef struct
{
  struct sockaddr_in addr;
  ssl_sock_t ssl_sock;
} ws_client_t;

void COLD init_ws(ws_client_t *ws);
bool HOT  handle_ws_connection_event(const ws_client_t *ws);
void HOT  handle_ws_event(const ws_client_t *ws);
void COLD free_ws(const ws_client_t *ws);

#endif