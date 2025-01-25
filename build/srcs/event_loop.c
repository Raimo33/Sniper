/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:40:24 by craimond          #+#    #+#             */
/*   Updated: 2025/01/25 14:32:07 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/event_loop.h"

void init_event_loop(event_loop_ctx_t *restrict ctx)
{
  ctx->epoll_fd = epoll_create1(0);
  const uint8_t signal_events = EPOLLIN | EPOLLONESHOT | EPOLLET;
  const uint8_t socket_events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLERR | EPOLLET;
  const uint8_t log_events = EPOLLOUT | EPOLLHUP | EPOLLERR | EPOLLET;
  
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_ADD, SIG_FILENO, &(struct epoll_event) {
    .events = signal_events,
    .data = { .fd = SIG_FILENO }
  });

  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_ADD, WS_FILENO, &(struct epoll_event) {
    .events = socket_events,
    .data = { .fd = WS_FILENO }
  });
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_ADD, FIX_FILENO, &(struct epoll_event) {
    .events = socket_events,
    .data = { .fd = FIX_FILENO }
  });
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_ADD, REST_FILENO, &(struct epoll_event) {
    .events = socket_events,
    .data = { .fd = REST_FILENO }
  });

  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_ADD, DNS_FILENO, &(struct epoll_event) {
    .events = socket_events,
    .data = { .fd = DNS_FILENO }
  });

  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_ADD, LOG_FILENO, &(struct epoll_event) {
    .events = log_events,
    .data = { .fd = LOG_FILENO }
  });
}

void establish_connections(const event_loop_ctx_t *restrict ctx, const fix_client_t *fix, const ws_client_t *ws, const rest_client_t *rest)
{
  struct epoll_event events[MAX_EVENTS] ALIGNED(64) = {0};
  struct epoll_event *event;
  uint8_t conn_count = 0;
  uint8_t n;

  //TODO implement DNS resolution concurrently in the same loop (otherwise signals and logs might not be handled)
  //TODO maybe notificate the client sockets with their domain names resolved so that epollet will catch the events

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
        case WS_FILENO:
          conn_count += handle_ws_connection(ws, event->events);
          break;
        case FIX_FILENO:
          conn_count += handle_fix_connection(fix, event->events);
          break;
        case REST_FILENO:
          conn_count += handle_rest_connection(rest, event->events);
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

void listen_events(const event_loop_ctx_t *restrict ctx, const fix_client_t *fix, const ws_client_t *ws, const rest_client_t *rest)
{
  //TODO: Implement this (take inspiration from establish_connections)
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