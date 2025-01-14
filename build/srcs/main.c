/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:07:42 by craimond          #+#    #+#             */
/*   Updated: 2025/01/14 18:55:35 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "headers/logger.h"
# include "headers/signals.h"
# include "headers/fix.h"
# include "headers/ws.h"
# include "headers/rest.h"
# include "headers/event_loop.h"

//https://developers.binance.com/docs/binance-spot-api-docs/faqs/spot_glossary

//epoll (linux event loop)
//non-blocking sockets
//custom JSON parser
//jemalloc (faster malloc)
//non-blocking DNS, dns cache
//high-resolution timers
//in-memory order books (binary heap)
//preallocated stack memory
//CPU caching
//SIMD (Single Instruction, Multiple Data)
//memory alignment
//async logging (in-memory ring buffer, write vectorized)
//GnuTLS (TLS 1.3) lightweigth
//TCP_NODELAY (disable Nagle's algorithm)
//timerfd_create() per timers con epoll
//fallocate() per preallocare spazio su disco

//TODO error checking per tutte le funzioni con goto, oppure assert
int32_t main(void)
{
  ssl_data_t ssl_data;
  event_loop_ctx_t loop;
  fix_client_t fix;
  ws_client_t ws;
  rest_client_t rest;

  init_logger();
  init_signals();
  init_ssl(&ssl_data);
  init_fix(&fix);
  init_ws(&ws);
  init_rest(&rest);
  init_event_loop(&loop);

  start_event_loop(&loop, &fix, &ws, &rest, &ssl_data);

  wolfSSL_Cleanup();
  free_fix(fix);
  free_ws(ws);
  free_rest(rest);
  close(loop->epoll_fd); //forse non basta
}