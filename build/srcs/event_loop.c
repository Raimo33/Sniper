/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:40:24 by craimond          #+#    #+#             */
/*   Updated: 2025/02/02 12:18:56 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/event_loop.h"

void init_event_loop(event_loop_ctx_t *restrict ctx)
{
  ctx->epoll_fd = epoll_create1(0);

  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_ADD, SIG_FILENO, &(struct epoll_event) {
    .events = SIGNAL_EVENTS,
    .data = { .fd = SIG_FILENO }
  });

  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_ADD, WS_FILENO, &(struct epoll_event) {
    .events = TCP_EVENTS,
    .data = { .fd = WS_FILENO }
  });
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_ADD, FIX_FILENO, &(struct epoll_event) {
    .events = TCP_EVENTS,
    .data = { .fd = FIX_FILENO }
  });
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_ADD, REST_FILENO, &(struct epoll_event) {
    .events = TCP_EVENTS,
    .data = { .fd = REST_FILENO }
  });

  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_ADD, DNS_FILENO, &(struct epoll_event) {
    .events = UDP_EVENTS,
    .data = { .fd = DNS_FILENO }
  });

  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_ADD, LOG_FILENO, &(struct epoll_event) {
    .events = LOG_EVENTS,
    .data = { .fd = LOG_FILENO }
  });
}

void connect_clients(const event_loop_ctx_t *restrict ctx, clients_t *restrict clients, dns_resolver_t *restrict dns_resolver)
{
  struct epoll_event events[MAX_EVENTS] ALIGNED(16) = {0};
  struct epoll_event *event;
  uint8_t n_connections = 0;
  uint8_t n;

  while (LIKELY(n_connections < 3))
  {
    n = epoll_wait(ctx->epoll_fd, events, MAX_EVENTS, -1);
    for (event = events; n > 0; n--, event++)
    {    
      switch (event->data.fd)
      {
        case SIG_FILENO:
          handle_signal(event->events);
          break;
        case DNS_FILENO:
          handle_dns_responses(dns_resolver, event->events);
          break;
        case WS_FILENO:
          n_connections += handle_ws_connection(&clients->ws, event->events, dns_resolver);
          break;
        case FIX_FILENO:
          n_connections += handle_fix_connection(&clients->fix, event->events, dns_resolver);
          break;
        case REST_FILENO:
          n_connections += handle_rest_connection(&clients->rest, event->events, dns_resolver);
          break;
        case LOG_FILENO:
          handle_logs(event->events);
          break;
        default:
          UNREACHABLE;
      }
    }
  }
}

void setup_trading(const event_loop_ctx_t *restrict ctx, clients_t *restrict clients, graph_t *restrict graph)
{
  //TODO fetches exchange info, fetches user info, fills graph and other
  (void)ctx;
  (void)clients;
  (void)graph;
}

void trade(const event_loop_ctx_t *restrict ctx, clients_t *restrict clients, graph_t *restrict graph)
{
  //TODO: updates graph with live ws data, sends fix orders, heartbeats, periodical checks with rest
  (void)ctx;
  (void)clients;
  (void)graph;
}

void free_event_loop(const event_loop_ctx_t *restrict ctx)
{
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_DEL, SIG_FILENO, NULL);
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_DEL, WS_FILENO, NULL);
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_DEL, FIX_FILENO, NULL);
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_DEL, REST_FILENO, NULL);
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_DEL, LOG_FILENO, NULL);
  close(ctx->epoll_fd);
}