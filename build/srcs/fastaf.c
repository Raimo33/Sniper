/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fastaf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:07:42 by craimond          #+#    #+#             */
/*   Updated: 2025/02/07 11:29:20 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fastaf.h"

static app_resources_t *restrict app;

COLD static inline void free_logger_wrapper(void)       { free_logger(app->log_fd); }
COLD static inline void free_signals_wrapper(void)      { free_signals(app->sig_fd); }
COLD static inline void free_keys_wrapper(void)         { free_keys(&app->keys); }
COLD static inline void free_ssl_wrapper(void)          { free_ssl(app->ssl_ctx); }
COLD static inline void free_fix_wrapper(void)          { free_fix(&app->clients.fix); }
COLD static inline void free_ws_wrapper(void)           { free_ws(&app->clients.ws); }
COLD static inline void free_http_wrapper(void)         { free_http(&app->clients.http); }
COLD static inline void free_event_loop_wrapper(void)   { free_event_loop(app->epoll_fd); }
COLD static inline void free_graph_wrapper(void)        { free_graph(&app->graph); }

int32_t main(void)
{
  app_resources_t local_app = {0};
  app = &local_app;

  // set_fd_limit(MAX_FDS); #TODO attivare senza valgrind
  app->log_fd = init_logger();
  atexit(free_logger_wrapper);
  app->sig_fd = init_signals();
  atexit(free_signals_wrapper);
  init_keys(&app->keys);
  atexit(free_keys_wrapper);
  app->ssl_ctx = init_ssl();
  atexit(free_ssl_wrapper);
  init_fix(&app->clients.fix, &app->keys, app->ssl_ctx);
  atexit(free_fix_wrapper);
  init_ws(&app->clients.ws, app->ssl_ctx);
  atexit(free_ws_wrapper);
  init_http(&app->clients.http, &app->keys, app->ssl_ctx);
  atexit(free_http_wrapper);
  app->epoll_fd = init_event_loop(&app->clients, app->log_fd, app->sig_fd);
  atexit(free_event_loop_wrapper);
  init_graph(&app->graph);
  atexit(free_graph_wrapper);

  resolve_domains(&app->clients);
  connect_clients(app->epoll_fd, &app->clients, app->log_fd, app->sig_fd);
  // setup_trading(app->epoll_fd, &app->clients, app->log_fd, app->sig_fd);
  // trade(app->epoll_fd, &app->clients, app->log_fd, app->sig_fd);
  exit(EXIT_SUCCESS);
}