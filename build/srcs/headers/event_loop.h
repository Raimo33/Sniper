/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:42:49 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 18:21:04 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_LOOP_H
# define EVENT_LOOP_H

# include <sys/epoll.h>
# include <stdbool.h>

# include "system_tweaks.h"
# include "extensions.h"
# include "signals.h"
# include "ws.h"
# include "fix.h"
# include "rest.h"
# include "logger.h"
# include "graph.h"

# define MAX_EVENTS 128

# define SIGNAL_EVENTS  EPOLLIN | EPOLLONESHOT | EPOLLET
# define TCP_EVENTS     EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLRDHUP | EPOLLERR | EPOLLET
# define LOG_EVENTS     EPOLLOUT | EPOLLHUP | EPOLLERR | EPOLLET

typedef struct
{
  ws_client_t ws;
  fix_client_t fix;
  rest_client_t rest;
} clients_t;

typedef struct
{
  uint8_t epoll_fd;
  clients_t *clients;
  uint8_t log_fd;
  uint16_t sig_fd;
} event_loop_ctx_t;

typedef void (*EventHandler)(uint8_t fd, uint32_t events, void *data);

typedef struct
{
  EventHandler handler;
  void *data;
} HandlerEntry;

extern HandlerEntry handlers[MAX_FDS];

COLD uint16_t init_event_loop(clients_t *restrict clients, const uint8_t log_fd, const uint8_t signal_fd);
COLD void resolve_domains(clients_t *restrict clients);
COLD void connect_clients(const uint8_t epoll_fd, clients_t *restrict clients, const uint8_t log_fd, const uint8_t signal_fd);
COLD void setup_trading(const uint8_t epoll_fd, clients_t *restrict clients, const uint8_t log_fd, const uint8_t signal_fd);
COLD void trade(const uint8_t epoll_fd, clients_t *restrict clients, const uint8_t log_fd, const uint8_t signal_fd);
COLD void free_event_loop(const uint8_t epoll_fd);

#endif