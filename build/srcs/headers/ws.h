/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:53:00 by craimond          #+#    #+#             */
/*   Updated: 2025/01/13 21:02:18 by craimond         ###   ########.fr       */
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

# include "ssl.h"

# define WS_HOST "data-stream.binance.com"
# define WS_PORT 9443U
# define WS_PORT_STR "9443"
# define WS_FILENO 5U
# define WS_PATH "/stream?streams=usdcusdt@trade/usdpusdt@trade/usdtdai@trade/fdusdusdt@trade/fdusdusdc@trade/tusdusdt@trade/eureuri@trade"

typedef struct
{
  const struct sockaddr_in addr;
  const ssl_sock_t ssl_sock;
} ws_client_t;

void init_ws(ws_client_t *const ws, ssl_data_t *ssl_data);
//TODO handler

#endif