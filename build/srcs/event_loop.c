/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:40:24 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 11:20:03 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "event_loop.h"

HandlerEntry handlers[MAX_FDS] = {0};

HOT static inline uint8_t get_connected_clients(const clients_t *restrict clients);

uint16_t init_event_loop(clients_t *restrict clients, const uint8_t log_fd, const uint8_t signal_fd)
{
  const uint8_t epoll_fd = epoll_create1_p(0);

  epoll_ctl_p(epoll_fd, EPOLL_CTL_ADD, signal_fd, &(struct epoll_event) {
    .events = SIGNAL_EVENTS,
    .data = { .fd = signal_fd }
  });

  epoll_ctl_p(epoll_fd, EPOLL_CTL_ADD, clients->ws.sock_fd, &(struct epoll_event) {
    .events = TCP_EVENTS,
    .data = { .fd = clients->ws.sock_fd }
  });
  epoll_ctl_p(epoll_fd, EPOLL_CTL_ADD, clients->fix.sock_fd, &(struct epoll_event) {
    .events = TCP_EVENTS,
    .data = { .fd = clients->fix.sock_fd }
  });
  epoll_ctl_p(epoll_fd, EPOLL_CTL_ADD, clients->http.sock_fd, &(struct epoll_event) {
    .events = TCP_EVENTS,
    .data = { .fd = clients->http.sock_fd }
  });

  epoll_ctl_p(epoll_fd, EPOLL_CTL_ADD, log_fd, &(struct epoll_event) {
    .events = LOG_EVENTS,
    .data = { .fd = log_fd }
  });

  return epoll_fd;
}

void resolve_domains(clients_t *restrict clients)
{
  static const struct addrinfo hints = {
    .ai_family = AF_UNSPEC,
    .ai_socktype = SOCK_STREAM,
    .ai_protocol = IPPROTO_TCP
  };

  struct gaicb ws_request = {
    .ar_name = WS_HOST,
    .ar_service = WS_PORT,
    .ar_request = &hints
  };

  struct gaicb fix_request = {
    .ar_name = FIX_HOST,
    .ar_service = FIX_PORT,
    .ar_request = &hints
  };

  struct gaicb rest_request = {
    .ar_name = HTTP_HOST,
    .ar_service = HTTP_PORT,
    .ar_request = &hints
  };

  struct gaicb *requests[] = { &ws_request, &fix_request, &rest_request };

  getaddrinfo_a_p(GAI_WAIT, requests, ARR_LEN(requests), NULL);

  clients->ws.addr = *(struct sockaddr_in *)ws_request.ar_result->ai_addr;
  clients->fix.addr = *(struct sockaddr_in *)fix_request.ar_result->ai_addr;
  clients->http.addr = *(struct sockaddr_in *)rest_request.ar_result->ai_addr;

  freeaddrinfo(ws_request.ar_result);
  freeaddrinfo(fix_request.ar_result);
  freeaddrinfo(rest_request.ar_result);
}

void connect_clients(const uint8_t epoll_fd, clients_t *restrict clients, const uint8_t log_fd, const uint8_t signal_fd)
{
  struct epoll_event events[MAX_EVENTS] ALIGNED(16) = {0};
  struct epoll_event *event;
  uint8_t n;

  handlers[signal_fd]             = (HandlerEntry){ handle_signal, NULL };
  handlers[clients->ws.sock_fd]   = (HandlerEntry){ handle_ws_connection, &clients->ws };
  handlers[clients->fix.sock_fd]  = (HandlerEntry){ handle_fix_connection, &clients->fix };
  handlers[clients->http.sock_fd] = (HandlerEntry){ handle_http_connection, &clients->http };
  handlers[log_fd]                = (HandlerEntry){ handle_logs, NULL };

  //TODO tutti i connect qui, prima di epoll

  while (LIKELY(get_connected_clients(clients) < 3))
  {
    n = epoll_wait_p(epoll_fd, events, MAX_EVENTS, -1);
    for (event = events; n > 0; n--, event++)
      handlers[event->data.fd].handler(event->data.fd, event->events, handlers[event->data.fd].data);
  }
}

void setup_trading(const uint8_t epoll_fd, clients_t *restrict clients, const uint8_t log_fd, const uint8_t signal_fd)
{
  (void)epoll_fd;
  (void)clients;
  (void)log_fd;
  (void)signal_fd;
  //TODO fetches exchange info, fetches user info, fills graph and other
}

void trade(const uint8_t epoll_fd, clients_t *restrict clients, const uint8_t log_fd, const uint8_t signal_fd)
{
  (void)epoll_fd;
  (void)clients;
  (void)log_fd;
  (void)signal_fd;
  //TODO: updates graph with live ws data, sends fix orders, heartbeats, periodical checks with http
}

static uint8_t get_connected_clients(const clients_t *restrict clients)
{
  return (clients->ws.status >= CONNECTED) + (clients->fix.status >= CONNECTED) + (clients->http.status >= CONNECTED);
}

void free_event_loop(const uint8_t epoll_fd)
{
  close(epoll_fd);
}