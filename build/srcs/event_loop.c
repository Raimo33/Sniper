/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:40:24 by craimond          #+#    #+#             */
/*   Updated: 2025/01/10 17:52:26 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/event_loop.h"

uint8_t init_event_loop(void)
{
  const uint8_t epoll_fd = epoll_create1(0);
  const uint8_t signal_events = EPOLLIN | EPOLLET;
  const uint8_t socket_events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLERR | EPOLLET;
  const uint8_t log_events = EPOLLOUT | EPOLLHUP | EPOLLERR | EPOLLET;
  
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, SIG_FD, &(struct epoll_event) {
    .events = signal_events,
    .data = { .fd = SIG_FD }
  });

  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, WS_FD, &(struct epoll_event) {
    .events = socket_events,
    .data = { .fd = WS_FD }
  });
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, FIX_FD, &(struct epoll_event) {
    .events = socket_events,
    .data = { .fd = FIX_FD }
  });
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, REST_FD, &(struct epoll_event) {
    .events = socket_events,
    .data = { .fd = REST_FD }
  });

  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, LOG_FD, &(struct epoll_event) {
    .events = log_events,
    .data = { .fd = LOG_FD }
  });

  return epoll_fd;
}


/*

TODO

ET mode requires specific handling:

Non-blocking sockets
Complete reads/writes
EAGAIN handling
Error checking

*/

void start_event_loop(const uint8_t epoll_fd)
{
  struct epoll_event events[MAX_EVENTS] = {0};

  while (true)
  {
    uint8_t n_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

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