/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:40:24 by craimond          #+#    #+#             */
/*   Updated: 2025/01/17 20:32:21 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/event_loop.h"

void init_event_loop(event_loop_ctx_t *ctx)
{
  ctx->epoll_fd = epoll_create1(0);
  const uint8_t signal_events = EPOLLIN | EPOLLONESHOT | EPOLLPRI | EPOLLET;
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

// panic(); //TODO con jump o goto o assembly, implementare anche assert che chiama panic
void establish_connections(const event_loop_ctx_t *ctx, const fix_client_t *fix, const ws_client_t *ws, const rest_client_t *rest)
{
  struct epoll_event events[MAX_EVENTS] = {0};
  struct epoll_event *event;
  uint8_t connected = 0;
  uint8_t n;

  while (LIKELY(connected < 3))
  {
    n = epoll_wait(ctx->epoll_fd, events, MAX_EVENTS, -1);
    event = events;
    while (n--)
    {
      PREFETCHR(event + 1, L0);
      //TODO computed gotos (1-2% performance increase)
      switch (event.events)
      {
        case EPOLLERR: FALLTHROUGH;
        case EPOLLHUP: FALLTHROUGH;
        case EPOLLRDHUP:
          panic(STR_LEN_PAIR("Connection error")); //TODO eventualmente getsockopt per capire l'errore
          break;
      }
      switch (event.data.fd) //TODO computed gotos (1-2% performance increase)
      {
        case SIG_FILENO:
          panic(STR_LEN_PAIR("Signal intercepted"));
          break;
        case WS_FILENO:
          connected += handle_ws_connection_event(ws);
          break;
        case FIX_FILENO:
          connected += handle_fix_connection_event(fix);
          break;
        case REST_FILENO:
          connected += handle_rest_connection_event(rest);
          break;
        case LOG_FILENO:
          flush_logs();
          break;
      }
      event++;
    }
  }
}

void listen_events(const event_loop_ctx_t *ctx, const fix_client_t *fix, const ws_client_t *ws, const rest_client_t *rest)
{
  struct epoll_event events[MAX_EVENTS] = {0};
  struct epoll_event *event;
  uint8_t n;

  while (true)
  {
    n = epoll_wait(ctx->epoll_fd, events, MAX_EVENTS, -1);
    event = events;
    while (n--)
    {
      PREFETCHR(event + 1, L0);
      switch (event.events) //TODO computed gotos (1-2% performance increase)
      {
        case EPOLLERR: FALLTHROUGH;
        case EPOLLHUP: FALLTHROUGH;
        case EPOLLRDHUP:
          panic(STR_LEN_PAIR("Connection error")); //TODO eventualmente getsockopt per capire l'errore
          break;
      }
      switch (event.data.fd) //TODO computed gotos (1-2% performance increase)
      {
        case SIG_FILENO:
          panic(STR_LEN_PAIR("Signal intercepted"));
        case WS_FILENO:
          handle_ws_event(ws, event.events);
          break;
        case FIX_FILENO:
          handle_fix_event(fix, event.events);
          break;
        case REST_FILENO:
          handle_rest_event(rest, event.events);
          break;
        case LOG_FILENO:
          flush_logs();
          break;
      }
      event++;
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