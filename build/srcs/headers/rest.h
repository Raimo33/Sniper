/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:43 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 13:18:10 by craimond         ###   ########.fr       */
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
# include <yyjson.h>

# include "extensions.h"
# include "fast_ssl.h"
# include "keys.h"
# include "dns_resolver.h"
# include "message_broker.h"
# include "http_serializer.h"
# include "decompression.h"
# include "graph.h"

# define REST_HOST "api.binance.com"
# define REST_PORT 8080
# define REST_FILENO 6
# define REST_READ_BUFFER_SIZE 4096
# define REST_WRITE_BUFFER_SIZE 4096

# define REST_KEEPALIVE_IDLE  5
# define REST_KEEPALIVE_INTVL 1
# define REST_KEEPALIVE_CNT   3

typedef struct
{
  struct sockaddr_in addr;
  SSL *ssl;
  keys_t *keys;
  char *write_buffer;
  char *read_buffer;
  http_response_t http_response;
  uint32_t write_offset;
  uint32_t read_offset;
  bool connected;
} rest_client_t;

COLD void init_rest(rest_client_t *restrict client, keys_t *restrict keys, SSL_CTX *restrict ssl_ctx);
HOT void handle_rest_connection(rest_client_t *restrict client, const uint32_t events, dns_resolver_t *restrict resolver);
HOT void handle_rest_setup(rest_client_t *restrict client, const uint32_t events, graph_t *restrict graph);
HOT void handle_rest_trading(rest_client_t *restrict client, const uint32_t events, graph_t *restrict graph);
COLD void free_rest(rest_client_t *restrict client);

#endif
