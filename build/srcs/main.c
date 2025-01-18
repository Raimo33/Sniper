/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:07:42 by craimond          #+#    #+#             */
/*   Updated: 2025/01/18 09:57:48 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/logger.h"
#include "headers/errors.h"
#include "headers/signals.h"
#include "headers/fix.h"
#include "headers/ws.h"
#include "headers/rest.h"
#include "headers/event_loop.h"

//https://developers.binance.com/docs/binance-spot-api-docs/faqs/spot_glossary

void *cleanup_label = NULL;

int32_t main(void)
{
  keys_t keys;
  event_loop_ctx_t loop;
  fix_client_t fix;
  ws_client_t ws;
  rest_client_t rest;
  cleanup_label = &&cleanup;

  init_logger();
  init_signals();
  init_keys(&keys);
  init_ssl();
  init_fix(&fix, &keys);
  init_ws(&ws);
  init_rest(&rest, &keys);
  init_event_loop(&loop);

  establish_connections(&loop, &fix, &ws, &rest);
  listen_events(&loop, &fix, &ws, &rest);

cleanup:
  free_event_loop(&loop);
  free_rest(&rest);
  free_ws(&ws);
  free_fix(&fix);
  free_ssl();
  free_keys(&keys);
  free_signals();
  free_logger();  
}