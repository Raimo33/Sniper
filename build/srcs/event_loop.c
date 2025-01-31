/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:40:24 by craimond          #+#    #+#             */
/*   Updated: 2025/01/31 18:29:00 by craimond         ###   ########.fr       */
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

void establish_connections(const event_loop_ctx_t *restrict ctx, fix_client_t *fix_client, ws_client_t *ws_client, rest_client_t *rest_client, dns_resolver_t *dns_resolver)
{
  struct epoll_event events[MAX_EVENTS] ALIGNED(16) = {0};
  struct epoll_event *event;
  uint8_t conn_count = 0;
  uint8_t n;

  while (LIKELY(conn_count < 3))
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
          conn_count += handle_ws_connection(ws_client, event->events, dns_resolver);
          break;
        case FIX_FILENO:
          conn_count += handle_fix_connection(fix_client, event->events, dns_resolver);
          break;
        case REST_FILENO:
          conn_count += handle_rest_connection(rest_client, event->events, dns_resolver);
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

void listen_events(const event_loop_ctx_t *restrict ctx, const fix_client_t *fix_client, const ws_client_t *ws_client, const rest_client_t *rest_client)
{
  //TODO: Implement this (take inspiration from establish_connections)
  (void)ctx;
  (void)fix_client;
  (void)ws_client;
  (void)rest_client;
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