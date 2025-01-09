/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:40:24 by craimond          #+#    #+#             */
/*   Updated: 2025/01/09 21:35:45 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/event_loop.h"
#include "headers/logging.h"
#include "headers/signals.h"

uint8_t init_event_loop(void)
{
  uint8_t epoll_fd = epoll_create1(0);

  //TODO epoll_ctl per tutti i file descriptor

  return epoll_fd;
}

//TODO studiare EPOLLET
void event_loop(const uint8_t epoll_fd)
{
  struct epoll_event events[MAX_EVENTS] = {0};

  while (!g_stop)
  {
    int n_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

    for (uint8_t i = 0; i < n_events; i++)
    {
      switch (events[i].data.fd)
      {
        case
      }
    }
  }

  close(epoll_fd);
}