/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:40:24 by craimond          #+#    #+#             */
/*   Updated: 2025/01/09 17:48:52 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/event_loop.h"

void event_loop(void)
{
  uint8_t epoll_fd;
  struct epoll_event events[MAX_EVENTS];
  uint8_t n_events;

  epoll_fd = epoll_create1(0);

  /* Add sockets/fds to monitor */
  /* TODO: Add WebSocket connection fd */

  while (//TODO segnale di terminazione)
  {
    n_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
      
    for (uint8_t i = 0; i < n_events; i++)
    {
      /* Handle events based on fd type */
      /* TODO: Add WebSocket message handling */
    }
  }

  close(epoll_fd);
}