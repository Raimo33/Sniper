/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_broker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:17:03 by craimond          #+#    #+#             */
/*   Updated: 2025/02/03 22:36:17 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/message_broker.h"

bool try_ssl_send(SSL *restrict ssl, char *restrict buffer, const uint32_t len, uint32_t *offset)
{
  const uint32_t ret = SSL_write(ssl, buffer + *offset, len);
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
  const uint32_t ret = SSL_read(ssl, buffer + *offset, buffer_size - *offset);
  if (UNLIKELY(ret <= 0))
    return false;

  *offset += ret;
  fast_assert(*offset < buffer_size, "Response too big");
  if (!is_full_http_response(buffer, buffer_size, *offset))
    return false;

  const uint32_t bytes_deserialized = deserialize_http_response(buffer, http_response, buffer_size);
  memmove(buffer, buffer + bytes_deserialized, *offset - bytes_deserialized);
  *offset -= bytes_deserialized;
  return true;
}

bool try_udp_recv(uint16_t sock_fd, char *restrict buffer, const uint32_t buffer_size, const char *restrict server_ip, const uint16_t server_ip_len)
{
  struct sockaddr_in src_addr;
  socklen_t src_addr_len = sizeof(src_addr);
  char src_ip[INET_ADDRSTRLEN];

  const uint16_t ret = recvfrom(sock_fd, buffer, buffer_size, MSG_DONTWAIT, (struct sockaddr *)&src_addr, &src_addr_len);
  inet_ntop(AF_INET, &src_addr.sin_addr, src_ip, INET_ADDRSTRLEN);
  fast_assert(memcmp(src_ip, server_ip, server_ip_len) == 0, "Sender not recognized");

  return ret > 0;
}
