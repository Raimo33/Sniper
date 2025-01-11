/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:51 by craimond          #+#    #+#             */
/*   Updated: 2025/01/11 10:20:15 by craimond         ###   ########.fr       */
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

# include "headers/ssl.h"

# define FIX_HOST "fix-oe.binance.com"
# define FIX_PORT 9000U

typedef struct
{
  const uint8_t fd;
  const struct sockaddr_in addr;
  const ssl_t ssl;
} fix_client_t;

void init_fix(fix_client_t *const fix);
//TODO handler

#endif
