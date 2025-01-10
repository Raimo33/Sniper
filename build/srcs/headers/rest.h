/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:43 by craimond          #+#    #+#             */
/*   Updated: 2025/01/10 18:20:12 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REST_H
# define REST_H

# include <sys/socket.h>
# include <sys/types.h>
# include <sys/fcntl.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <netdb.h>

# define REST_FD 6U
# define REST_HOST "api.binance.com"
# define REST_PORT 8080

void init_rest(void);
//TODO handler

#endif
