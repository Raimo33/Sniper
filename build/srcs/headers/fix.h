/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:51 by craimond          #+#    #+#             */
/*   Updated: 2025/01/14 18:54:55 by craimond         ###   ########.fr       */
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
  const struct sockaddr_in addr;
  const ssl_sock_t ssl_sock;
} fix_client_t;

void init_fix(fix_client_t *fix);
void free_fix(const fix_client_t *fix);
//TODO handler

#endif
