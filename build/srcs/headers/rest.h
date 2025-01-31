/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:43 by craimond          #+#    #+#             */
/*   Updated: 2025/01/31 17:15:52 by craimond         ###   ########.fr       */
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
# include "fast_ssl.h"
# include "keys.h"
# include "dns_resolver.h"
# include "message_broker.h"
# include "http_parser.h"
# include "compression.h"

# define REST_HOST "api.binance.com"
# define REST_PORT 8080
# define REST_FILENO 6
# define REST_READ_BUFFER_SIZE 262144
# define REST_WRITE_BUFFER_SIZE 4096

# define REST_KEEPALIVE_IDLE  5
# define REST_KEEPALIVE_INTVL 1
# define REST_KEEPALIVE_CNT   3

typedef struct
{
  struct sockaddr_in addr;
  SSL *ssl;
  const keys_t *keys;
  char write_buffer[REST_WRITE_BUFFER_SIZE];
  char read_buffer[REST_READ_BUFFER_SIZE];
  uint16_t write_offset;
  uint16_t read_offset;
  http_response_t http_response;
} rest_client_t;

void COLD init_rest(rest_client_t *restrict client, const keys_t *restrict keys, SSL_CTX *restrict ssl_ctx);
bool HOT handle_rest_connection(rest_client_t *restrict client, const uint8_t events, dns_resolver_t *restrict resolver);
void COLD free_rest(rest_client_t *restrict client);

#endif
