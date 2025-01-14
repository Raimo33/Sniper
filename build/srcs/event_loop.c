/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:40:24 by craimond          #+#    #+#             */
/*   Updated: 2025/01/14 20:55:24 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/event_loop.h"

static void establish_connection(const uint16_t fd, const struct sockaddr_in *addr, const ssl_sock_t *ssl_sock);

void init_event_loop(event_loop_ctx_t *ctx)
{
  ctx->epoll_fd = epoll_create1(0);
  const uint8_t signal_events = EPOLLIN | EPOLLET;
  const uint8_t socket_events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLERR | EPOLLET;
  const uint8_t log_events = EPOLLOUT | EPOLLHUP | EPOLLERR | EPOLLET;
  
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_ADD, SIG_FILENO, &(struct epoll_event) {
    .events = signal_events,
    .data = { .fd = SIG_FILENO }
  });

  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_ADD, ws->fd, &(struct epoll_event) {
    .events = socket_events,
    .data = { .fd = WS_FILENO }
  });
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_ADD, fix->fd, &(struct epoll_event) {
    .events = socket_events,
    .data = { .fd = FIX_FILENO }
  });
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_ADD, rest->fd, &(struct epoll_event) {
    .events = socket_events,
    .data = { .fd = REST_FILENO }
  });

  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_ADD, LOG_FILENO, &(struct epoll_event) {
    .events = log_events,
    .data = { .fd = LOG_FILENO }
  });
}

void establish_connections(const event_loop_ctx_t *ctx, const fix_client_t *fix, const ws_client_t *ws, const rest_client_t *rest)
{
  struct epoll_event events[MAX_EVENTS] = {0};
  uint8_t n_events;
  uint8_t i;

  while (true)
  {
    n_events = epoll_wait(ctx->epoll_fd, events, MAX_EVENTS, -1);
    for (i = 0; i < n_events; i++)
    {
      switch (events[i].data.fd)
      {
        case SIG_FILENO:
          panic(); //TODO con jump o goto o assembly, implementare anche assert che chiama panic
        case WS_FILENO:
          establish_ws_connection(ws);
          break;
        case FIX_FILENO:
          establish_fix_connection(fix);
          break;
        case REST_FILENO:
          establish_rest_connection(rest);
          break;
        case LOG_FILENO:
          flush_logs();
          break;
      }
    }
  }
}

void listen_events(const event_loop_ctx_t *ctx, const fix_client_t *fix, const ws_client_t *ws, const rest_client_t *rest)
{
  struct epoll_event events[MAX_EVENTS] = {0};
  uint8_t n_events;
  uint8_t i;

  while (true)
  {
    n_events = epoll_wait(ctx->epoll_fd, events, MAX_EVENTS, -1);
    for (i = 0; i < n_events; i++)
    {
      switch (events[i].data.fd)
      {
        case SIG_FILENO:
          panic();
        case WS_FILENO:
          handle_ws_event(ws);
          break;
        case FIX_FILENO:
          handle_fix_event(fix);
          break;
        case REST_FILENO:
          handle_rest_event(rest);
          break;
        case LOG_FILENO:
          flush_logs();
          break;
      }
    }
  }
}

void free_event_loop(const event_loop_ctx_t *ctx)
{
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_DEL, SIG_FILENO, NULL);
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_DEL, WS_FILENO, NULL);
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_DEL, FIX_FILENO, NULL);
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_DEL, REST_FILENO, NULL);
  epoll_ctl(ctx->epoll_fd, EPOLL_CTL_DEL, LOG_FILENO, NULL);
  close(ctx->epoll_fd);
}