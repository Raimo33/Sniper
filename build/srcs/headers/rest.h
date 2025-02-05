/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:43 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 16:33:10 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REST_H
# define REST_H

# include <stdint.h>
# include <yyjson.h>

# include "client_common.h"
# include "extensions.h"
# include "keys.h"
# include "decompression.h"
# include "graph.h"

# define REST_HOST "api.binance.com"
# define REST_PORT 8080
# define REST_READ_BUFFER_SIZE 4096
# define REST_WRITE_BUFFER_SIZE 4096

# define REST_KEEPALIVE_IDLE  5
# define REST_KEEPALIVE_INTVL 1
# define REST_KEEPALIVE_CNT   3

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
} rest_client_t;

COLD void init_rest(rest_client_t *restrict client, keys_t *restrict keys, SSL_CTX *restrict ssl_ctx);
HOT void handle_rest_connection(const uint16_t fd, const uint32_t events, void *data);
HOT void handle_rest_setup(const uint16_t fd, const uint32_t events, void *data);
HOT void handle_rest_trading(const uint16_t fd, const uint32_t events, void *data);
COLD void free_rest(rest_client_t *restrict client);

#endif
