/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_common.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 14:12:17 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 18:09:44 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_COMMON_H
# define CLIENT_COMMON_H

# include <stdint.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <netdb.h>

# include "fast_ssl.h"
# include "http_serializer.h"
# include "message_broker.h"

typedef enum: uint8_t {DISCONNECTED, CONNECTED, TRADING} client_status_t;

#endif