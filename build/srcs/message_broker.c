/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_broker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:17:03 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 22:07:16 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "message_broker.h"

bool try_ssl_send(SSL *restrict ssl, char *restrict buffer, const uint32_t len, uint32_t *offset)
{
  printf("out buffer: %s\n", buffer);
  const uint32_t ret = SSL_write_p(ssl, buffer + *offset, len);
  if (UNLIKELY(ret <= 0))
    return false;
  
  *offset += ret;
  if (*offset != len)
    return false;

  memset(buffer, 0, len);
  *offset = 0;
  return true;
}

bool try_ssl_recv_http(SSL *restrict ssl, char *restrict buffer, const uint32_t buffer_size, uint32_t *offset, http_response_t *restrict http_response)
{
  const uint32_t ret = SSL_read_p(ssl, buffer + *offset, buffer_size - *offset);
  if (UNLIKELY(ret <= 0))
    return false;
  printf("in buffer: %s\n", buffer);
  *offset += ret;
  fast_assert(*offset <= buffer_size, "Response too big");
  if (!is_full_http_response(buffer, buffer_size, *offset))
    return false;

  const uint32_t bytes_deserialized = deserialize_http_response(buffer, http_response, buffer_size);
  memmove(buffer, buffer + bytes_deserialized, *offset - bytes_deserialized);
  *offset -= bytes_deserialized;
  return true;
}
