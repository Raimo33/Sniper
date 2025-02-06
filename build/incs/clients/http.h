/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:43 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 11:31:54 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_H
# define HTTP_H

# include <stdint.h>
# include <yyjson.h>

# include "primitives/network_primitives.h"
# include "primitives/yyjson_primitives.h"
# include "client_common.h"
# include "extensions.h"
# include "keys.h"
# include "parsing/decompression.h"
# include "graph.h"

# define HTTP_HOST "api.binance.com"
# define HTTP_PORT "8080"
# define HTTP_READ_BUFFER_SIZE 4096
# define HTTP_WRITE_BUFFER_SIZE 4096
# define HTTP_KEEPALIVE_IDLE  5
# define HTTP_KEEPALIVE_INTVL 1
# define HTTP_KEEPALIVE_CNT   3

typedef struct
{
  uint16_t sock_fd;
  struct sockaddr_in addr;
  SSL *ssl;
  keys_t *keys;
  char *write_buffer;
  char *read_buffer;
  http_response_t http_response;
  uint32_t write_offset;
  uint32_t read_offset;
  client_status_t status;
} http_client_t;

COLD void init_http(http_client_t *restrict client, keys_t *restrict keys, SSL_CTX *restrict ssl_ctx);
HOT void handle_http_connection(const uint8_t fd, const uint32_t events, void *data);
HOT void handle_http_setup(const uint8_t fd, const uint32_t events, void *data);
HOT void handle_http_trading(const uint8_t fd, const uint32_t events, void *data);
COLD void free_http(http_client_t *restrict client);

#endif
