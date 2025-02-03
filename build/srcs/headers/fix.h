/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:51 by craimond          #+#    #+#             */
/*   Updated: 2025/02/03 22:09:06 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIX_H
# define FIX_H

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
# include "http_serializer.h"
# include "fix_serializer.h"
# include "fixed_point.h"
# include "graph.h"

# define FIX_HOST "fix-oe.binance.com"
# define FIX_PORT 9000
# define FIX_FILENO 4
# define FIX_KEEPALIVE_IDLE 300
# define FIX_KEEPALIVE_INTVL 30
# define FIX_KEEPALIVE_CNT 3
# define FIX_READ_BUFFER_SIZE 4096
# define FIX_WRITE_BUFFER_SIZE 4096
# define FIX_HEARTBEAT_INTERVAL 45
# define FIX_VERSION "FIX.4.4"
# define FIX_COMP_ID "FAF42"

//https://developers.binance.com/docs/binance-spot-api-docs/fix-api#message-components
//https://github.com/binance/binance-spot-api-docs/blob/master/fix-api.md
// https://github.com/binance/binance-spot-api-docs/blob/master/fix/schemas/spot-fix-oe.xml

typedef struct
{
  struct sockaddr_in addr;
  SSL *ssl;
  const keys_t *keys;
  char *write_buffer;
  char *read_buffer;
  // http_response_t http_response;
  uint32_t write_offset;
  uint32_t read_offset;
  uint64_t msg_seq_num;
  bool connected;
} fix_client_t;

COLD void init_fix(fix_client_t *restrict client, const keys_t *restrict keys, SSL_CTX *restrict ssl_ctx);
HOT void handle_fix_connection(fix_client_t *restrict client, const uint8_t events, dns_resolver_t *restrict resolver);
HOT void handle_fix_setup(fix_client_t *restrict client, const uint8_t events, graph_t *restrict graph);
HOT void handle_fix_trading(fix_client_t *restrict client, const uint8_t events, graph_t *restrict graph);
COLD void free_fix(fix_client_t *restrict client);

#endif
