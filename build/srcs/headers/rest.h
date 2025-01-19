/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:43 by craimond          #+#    #+#             */
/*   Updated: 2025/01/19 19:17:20 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REST_H
# define REST_H

# include <stdint.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <netdb.h>

# include "extensions.h"
# include "ssl.h"
# include "shared_buffers.h"

# define REST_HOST "api.binance.com"
# define REST_PORT 8080
# define REST_FILENO 6

# define REST_KEEPALIVE_IDLE  5
# define REST_KEEPALIVE_INTVL 1
# define REST_KEEPALIVE_CNT   3

typedef struct
{
  struct sockaddr_in addr;
  SSL *ssl;
  const keys_t *keys;
} rest_client_t;

void COLD init_rest(rest_client_t *restrict rest, const keys_t *restrict keys, const WOLFSSL_CTX *restrict ssl_ctx);
inline bool HOT handle_rest_connection(const rest_client_t *restrict rest, const char fd_state);
void COLD free_rest(const rest_client_t *restrict rest);

#endif
