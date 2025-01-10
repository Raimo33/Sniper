/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:07:42 by craimond          #+#    #+#             */
/*   Updated: 2025/01/10 18:24:46 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//https://developers.binance.com/docs/binance-spot-api-docs/faqs/spot_glossary

//TODO epoll, nonblocking sockets, custom JSON parser, jemalloc, nonblocking dns, high-resolution timers, in-memory order books (binary heap), preallocated stack memory, CPU caching, SIMD, memory alignment, async logging (in-memory ring buffer), dont use OpenSSL (heavy), TCP_NODELAY, Set appropriate SO_RCVBUF and SO_SNDBUF,

#include "headers/arb.h"

t_config g_config = {0};

//TODO error checking per tutte le funzioni con goto
int32_t main(void)
{
  event_loop_ctx_t loop;

  init_config();
  init_logger();
  init_signals();
  init_ws();
  init_fix();
  init_rest();
  init_event_loop(&loop);
  start_event_loop(&loop);
  cleanup(&loop);
}
