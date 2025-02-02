/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fastaf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:07:42 by craimond          #+#    #+#             */
/*   Updated: 2025/02/02 12:17:41 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/fastaf.h"

static app_resources_t *restrict app;

COLD static inline void free_logger_wrapper(void)       { free_logger(); }
COLD static inline void free_signals_wrapper(void)      { free_signals(); }
COLD static inline void free_keys_wrapper(void)         { free_keys(&app->keys); }
COLD static inline void free_ssl_wrapper(void)          { free_ssl(app->ssl_ctx); }
COLD static inline void free_fix_wrapper(void)          { free_fix(&app->clients.fix); }
COLD static inline void free_ws_wrapper(void)           { free_ws(&app->clients.ws); }
COLD static inline void free_rest_wrapper(void)         { free_rest(&app->clients.rest); }
COLD static inline void free_dns_resolver_wrapper(void) { free_dns_resolver(&app->dns_resolver); }
COLD static inline void free_event_loop_wrapper(void)   { free_event_loop(&app->loop); }
COLD static inline void free_graph_wrapper(void)        { free_graph(&app->graph); }

int32_t main(void)
{
  app_resources_t local_app = {0};
  app = &local_app;

  init_logger();
  atexit(free_logger_wrapper);
  init_signals();
  atexit(free_signals_wrapper);
  init_keys(&app->keys);
  atexit(free_keys_wrapper);
  init_ssl(&app->ssl_ctx);
  atexit(free_ssl_wrapper);
  init_fix(&app->clients.fix, &app->keys, app->ssl_ctx);
  atexit(free_fix_wrapper);
  init_ws(&app->clients.ws, app->ssl_ctx);
  atexit(free_ws_wrapper);
  init_rest(&app->clients.rest, &app->keys, app->ssl_ctx);
  atexit(free_rest_wrapper);
  init_dns_resolver(&app->dns_resolver);
  atexit(free_dns_resolver_wrapper);
  init_event_loop(&app->loop);
  atexit(free_event_loop_wrapper);
  init_graph(&app->graph);
  atexit(free_graph_wrapper);

  connect_clients(&app->loop, &app->clients, &app->dns_resolver);
  setup_trading(&app->loop, &app->clients, &app->graph);
  trade(&app->loop, &app->clients, &app->graph);
  exit(EXIT_SUCCESS);
}