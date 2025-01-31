/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_broker.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:16:23 by craimond          #+#    #+#             */
/*   Updated: 2025/01/31 16:57:25 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_BROKER_H
# define MESSAGE_BROKER_H

# include <stdint.h>
# include <stdbool.h>

# include "fast_ssl.h"
# include "http_parser.h"
# include "extensions.h"

bool try_ssl_send(SSL *restrict ssl, char *restrict buffer, const uint16_t len, uint16_t *offset);
bool try_ssl_recv_http(SSL *restrict ssl, char *restrict buffer, const uint16_t buffer_size, uint16_t *offset, http_response_t *restrict http_response);

#endif