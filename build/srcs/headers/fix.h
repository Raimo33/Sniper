/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:51 by craimond          #+#    #+#             */
/*   Updated: 2025/01/18 10:53:52 by craimond         ###   ########.fr       */
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
# include "ssl.h"
# include "http_parser.h"
# include "shared_buffers.h"

# define FIX_HOST "fix-oe.binance.com"
# define FIX_PORT 9000U
# define FIX_FILENO 4U

typedef struct
{
  struct sockaddr_in addr;
  ssl_sock_t ssl_sock;
  const keys_t *keys;
} fix_client_t;

void COLD init_fix(fix_client_t *fix, const keys_t *keys);
bool HOT  handle_fix_connection_event(const fix_client_t *fix);
void HOT  handle_fix_event(const fix_client_t *fix);
void COLD free_fix(const fix_client_t *fix);

#endif
