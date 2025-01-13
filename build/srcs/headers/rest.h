/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:43 by craimond          #+#    #+#             */
/*   Updated: 2025/01/13 19:16:06 by craimond         ###   ########.fr       */
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

# include "ssl.h"

# define REST_HOST "api.binance.com"
# define REST_PORT 8080U
# define REST_FILENO 6U

# define REST_KEEPALIVE_IDLE  5U
# define REST_KEEPALIVE_INTVL 1U
# define REST_KEEPALIVE_CNT   3U

typedef struct
{
  const struct sockaddr_in addr;
  const ssl_sock_t ssl_sock;
} rest_client_t;

void init_rest(rest_client_t *const rest);
//TODO handler

#endif
