/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 21:02:36 by craimond          #+#    #+#             */
/*   Updated: 2025/01/11 16:38:21 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/fix.h"
#include "headers/config.h"

static ssl_t setup_ssl(const uint16_t fd);

//TODO pool di connessioni
void init_fix(fix_client_t *const fix)
{
  fix->addr = (struct sockaddr_in){
    .sin_family = AF_INET,
    .sin_port = htons(FIX_PORT),
    .sin_addr = {
      .s_addr = inet_addr(FIX_HOST) //TODO getaddrinfo e dns resolve
    }
  };

  const uint16_t fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(bool){1}, sizeof(bool));

  fix->ssl = init_ssl(fd);

  connect(fd, (const struct sockaddr *)&fix->addr, sizeof(fix->addr));

  dup2(fd, FIX_FILENO);
  close(fd);
}

void free_fix(fix_client_t *const fix)
{
  cleanup_ssl(&fix->ssl);
  close(FIX_FILENO);
}