/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:40:24 by craimond          #+#    #+#             */
/*   Updated: 2025/01/14 14:57:15 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/event_loop.h"

static void establish_connection(const uint16_t fd, const struct sockaddr_in *const addr, const ssl_sock_t *const ssl_sock);

void init_event_loop(event_loop_ctx_t *const ctx)
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


/*

TODO

ET mode requires specific handling:

Non-blocking sockets
Complete reads/writes
EAGAIN handling
Error checking

*/

void start_event_loop(const event_loop_ctx_t *const ctx, const ws_client_t *const fix, const ws_client_t *const ws, const rest_client_t *const rest, const ssl_data_t *const ssl_data)
{
  struct epoll_event events[MAX_EVENTS] = {0};

  while (true)
  {
    uint8_t n_events = epoll_wait(ctx->epoll_fd, events, MAX_EVENTS, -1);

    for (uint8_t i = 0; i < n_events; i++)
    {
      switch (events[i].data.fd)
      {
        //TODO handlers
      }
    }
  }
}

//TODO trovare un modo per eseguirle una alla volta async ma solo all'inizio
// connect(fd, (const struct sockaddr *)addr, sizeof(*addr));
// wolfSSL_connect(ssl_sock->ssl);
// (SOLO PER WS) perform_ws_handshake(ws, ws_key);