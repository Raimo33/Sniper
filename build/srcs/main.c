/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:07:42 by craimond          #+#    #+#             */
/*   Updated: 2025/01/11 10:36:15 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
//SO_RECVBUF / SO_SENDBUF //TODO needed?

#include "headers/arb.h"

//TODO error checking per tutte le funzioni con goto
//TODO struttura grande chiamata clients?
int32_t main(void)
{
  config_t config;
  event_loop_ctx_t loop;
  ws_client_t ws;
  fix_client_t fix;
  rest_client_t rest;

  init_config(&config);
  const uint8_t log_fd = init_logger();
  const uint8_t sig_fd = init_signals();
  wolfSSL_Init();
  init_ws(&ws);
  init_fix(&fix);
  init_rest(&rest);
  init_event_loop(&loop, &ws, &fix, &rest);
  start_event_loop(&loop, &ws, &fix, &rest);
  cleanup(&loop, &ws, &fix, &rest);
}
