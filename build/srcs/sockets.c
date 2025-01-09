/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 21:25:14 by craimond          #+#    #+#             */
/*   Updated: 2025/01/09 21:33:55 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/sockets.h"

static void init_rest_socket(void);
static void init_ws_socket(void);
static void init_fix_socket(void);
static void set_non_blocking(const uint8_t sock_fd);

void init_sockets(void)
{
  
}

static void init_rest_socket(void)
{
  uint8_t sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct addrinfo hints = {0}; //TODO

  set_non_blocking(sock_fd);
}

static void set_non_blocking(const uint8_t sock_fd)
{
  uint8_t flags = fcntl(sock_fd, F_GETFL, 0);
  fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK);
}