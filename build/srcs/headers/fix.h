/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:51 by craimond          #+#    #+#             */
/*   Updated: 2025/01/16 15:04:10 by craimond         ###   ########.fr       */
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

# include "ssl.h"

# define FIX_HOST "fix-oe.binance.com"
# define FIX_PORT 9000U
# define FIX_FILENO 4U

typedef struct
{
  struct sockaddr_in addr;
  ssl_sock_t ssl_sock;
  const keys_t *keys;
} fix_client_t;

__attribute__((cold)) void init_fix(fix_client_t *fix, const keys_t *keys);
__attribute__((hot))  bool handle_fix_connection_event(const fix_client_t *fix, const uint32_t events);
__attribute__((hot))  void handle_fix_event(const fix_client_t *fix);
__attribute__((cold)) void free_fix(const fix_client_t *fix);

#endif
