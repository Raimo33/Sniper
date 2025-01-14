/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 21:02:36 by craimond          #+#    #+#             */
/*   Updated: 2025/01/14 18:54:42 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/fix.h"

//TODO pool di connessioni
void init_fix(fix_client_t *fix)
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
  init_ssl_socket(fd, &fix->ssl_sock);

  dup2(fd, FIX_FILENO);
  close(fd);
}

void free_fix(const fix_client_t *fix)
{
  free_ssl_socket(&fix->ssl_sock);
  close(FIX_FILENO);
}