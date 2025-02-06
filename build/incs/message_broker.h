/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_broker.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:16:23 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 11:00:56 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_BROKER_H
# define MESSAGE_BROKER_H

# include <stdint.h>
# include <stdbool.h>
# include <arpa/inet.h>

# include "fast_ssl.h"
# include "parsing/http_serializer.h"
# include "extensions.h"

HOT bool try_ssl_send(SSL *restrict ssl, char *restrict buffer, const uint32_t len, uint32_t *offset);
HOT bool try_ssl_recv_http(SSL *restrict ssl, char *restrict buffer, const uint32_t buffer_size, uint32_t *offset, http_response_t *restrict http_response);

#endif