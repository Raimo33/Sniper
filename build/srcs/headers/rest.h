/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:43 by craimond          #+#    #+#             */
/*   Updated: 2025/01/10 19:29:31 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REST_H
# define REST_H

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <netdb.h>

# define REST_FD 6U
# define REST_HOST "api.binance.com"
# define REST_PORT 8080U

# define REST_KEEPALIVE_IDLE  5U
# define REST_KEEPALIVE_INTVL 1U
# define REST_KEEPALIVE_CNT   3U

void init_rest(void);
//TODO handler

#endif
