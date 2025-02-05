/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:40:24 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 16:40:21 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/event_loop.h"

HandlerEntry handlers[MAX_FDS] = {0};

HOT static inline uint8_t get_connected_clients(const clients_t *restrict clients);

uint16_t init_event_loop(clients_t *restrict clients, const uint16_t log_fd, const uint16_t signal_fd)
{
  const uint16_t epoll_fd = epoll_create1(0);

  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, signal_fd, &(struct epoll_event) {
    .events = SIGNAL_EVENTS,
    .data = { .fd = signal_fd }
  });

  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clients->ws.sock_fd, &(struct epoll_event) {
    .events = TCP_EVENTS,
    .data = { .fd = clients->ws.sock_fd }
  });
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clients->fix.sock_fd, &(struct epoll_event) {
    .events = TCP_EVENTS,
    .data = { .fd = clients->fix.sock_fd }
  });
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clients->rest.sock_fd, &(struct epoll_event) {
    .events = TCP_EVENTS,
    .data = { .fd = clients->rest.sock_fd }
  });

  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, log_fd, &(struct epoll_event) {
    .events = LOG_EVENTS,
    .data = { .fd = log_fd }
  });

  return epoll_fd;
}

void connect_clients(const uint16_t epoll_fd, clients_t *restrict clients, const uint16_t log_fd, const uint16_t signal_fd)
{
  struct epoll_event events[MAX_EVENTS] ALIGNED(16) = {0};
  struct epoll_event *event;
  uint8_t n;

  handlers[signal_fd]             = (HandlerEntry){ handle_signal, NULL };
  handlers[clients->ws.sock_fd]   = (HandlerEntry){ handle_ws_connection, &clients->ws };
  handlers[clients->fix.sock_fd]  = (HandlerEntry){ handle_fix_connection, &clients->fix };
  handlers[clients->rest.sock_fd] = (HandlerEntry){ handle_rest_connection, &clients->rest };
  handlers[log_fd]                = (HandlerEntry){ handle_logs, NULL };

  while (LIKELY(get_connected_clients(clients) < 3))
  {
    n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    for (event = events; n > 0; n--, event++)
      handlers[event->data.fd].handler(event->data.fd, event->events, handlers[event->data.fd].data);
  }
}

void setup_trading(const uint16_t epoll_fd, clients_t *restrict clients, const uint16_t log_fd, const uint16_t signal_fd)
{
  (void)epoll_fd;
  (void)clients;
  (void)log_fd;
  (void)signal_fd;
  //TODO fetches exchange info, fetches user info, fills graph and other
}

void trade(const uint16_t epoll_fd, clients_t *restrict clients, const uint16_t log_fd, const uint16_t signal_fd)
{
  (void)epoll_fd;
  (void)clients;
  (void)log_fd;
  (void)signal_fd;
  //TODO: updates graph with live ws data, sends fix orders, heartbeats, periodical checks with rest
}

static uint8_t get_connected_clients(const clients_t *restrict clients)
{
  return (clients->ws.status >= CONNECTED) + (clients->fix.status >= CONNECTED) + (clients->rest.status >= CONNECTED);
}

void free_event_loop(const uint16_t epoll_fd)
{
  close(epoll_fd);
}