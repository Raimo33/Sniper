/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:51 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 11:22:09 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIX_H
# define FIX_H

# include <stdint.h>

# include "primitives/network_primitives.h"
# include "client_common.h"
# include "extensions.h"
# include "keys.h"
# include "parsing/fix_serializer.h"
# include "fixed_point.h"
# include "graph.h"

# define FIX_HOST "fix-oe.binance.com"
# define FIX_PORT "9000"
# define FIX_KEEPALIVE_IDLE 300
# define FIX_KEEPALIVE_INTVL 30
# define FIX_KEEPALIVE_CNT 3
# define FIX_READ_BUFFER_SIZE 4096
# define FIX_WRITE_BUFFER_SIZE 4096
# define FIX_VERSION "FIX.4.4"
# define FIX_COMP_ID "FAF42"
# define FIX_HEARTBEAT_INTERVAL "60"

//https://developers.binance.com/docs/binance-spot-api-docs/fix-api#message-components
//https://github.com/binance/binance-spot-api-docs/blob/master/fix-api.md
// https://github.com/binance/binance-spot-api-docs/blob/master/fix/schemas/spot-fix-oe.xml

typedef struct
{
  uint16_t sock_fd;
  struct sockaddr_in addr;
  SSL *ssl;
  keys_t *keys;
  char *write_buffer;
  char *read_buffer;
  // http_response_t http_response;
  uint32_t write_offset;
  uint32_t read_offset;
  uint64_t msg_seq_num;
  client_status_t status;
} fix_client_t;

COLD void init_fix(fix_client_t *restrict client, keys_t *restrict keys, SSL_CTX *restrict ssl_ctx);
HOT void handle_fix_connection(const uint8_t fd, const uint32_t events, void *data);
HOT void handle_fix_setup(const uint8_t fd, const uint32_t events, void *data);
HOT void handle_fix_trading(const uint8_t fd, const uint32_t events, void *data);
COLD void free_fix(fix_client_t *restrict client);

#endif
