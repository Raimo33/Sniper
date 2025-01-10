/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 21:02:36 by craimond          #+#    #+#             */
/*   Updated: 2025/01/10 21:33:52 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/fix.h"
#include "headers/config.h"

//TODO init wolfssl
//TODO pool di connessioni
void init_fix(void)
{
  const struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(FIX_PORT),
    .sin_addr = {
      .s_addr = inet_addr(FIX_HOST)
    }
  };

  const uint8_t fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(uint8_t){1}, sizeof(uint8_t));

  dup2(fd, FIX_FD);
  close(fd);

  connect(FIX_FD, (const struct sockaddr *)&addr, sizeof(addr));
}