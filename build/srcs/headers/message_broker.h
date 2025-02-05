/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_broker.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:16:23 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 21:56:34 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_BROKER_H
# define MESSAGE_BROKER_H

# include <stdint.h>
# include <stdbool.h>
# include <arpa/inet.h>

# include "wrappers.h"
# include "fast_ssl.h"
# include "http_serializer.h"
# include "extensions.h"

HOT bool try_ssl_send(SSL *restrict ssl, char *restrict buffer, const uint32_t len, uint32_t *offset);
HOT bool try_ssl_recv_http(SSL *restrict ssl, char *restrict buffer, const uint32_t buffer_size, uint32_t *offset, http_response_t *restrict http_response);
COLD bool try_udp_recv(uint16_t sock_fd, char *restrict buffer, const uint32_t buffer_size, const char *restrict server_ip, const uint16_t server_ip_len);

#endif