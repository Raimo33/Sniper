/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:53:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/10 21:20:19 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/ws.h"

//TODO pool di connessioni
void init_ws(void)
{
  const struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(WS_PORT),
    .sin_addr = {
      .s_addr = inet_addr(WS_HOST)
    }
  };
  
  const uint8_t fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(uint8_t){1}, sizeof(uint8_t));

  dup2(fd, WS_FD);
  close(fd);

  connect(WS_FD, (const struct sockaddr *)&addr, sizeof(addr));
}