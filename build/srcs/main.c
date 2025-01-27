/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:07:42 by craimond          #+#    #+#             */
/*   Updated: 2025/01/27 09:30:42 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/logger.h"
#include "headers/errors.h"
#include "headers/signals.h"
#include "headers/fix.h"
#include "headers/ws.h"
#include "headers/rest.h"
#include "headers/dns_resolver.h"
#include "headers/event_loop.h"
#include "headers/keys.h"
#include "headers/ssl.h"

//TODO
//fetch api to find all available pairs, with their fees and minimum order size
//Represent currencies as nodes and exchange rates as weighted edges.
//Precompute All Triangular Cycles 
//SIMD-Based Cycle Evaluation

//https://developers.binance.com/docs/binance-spot-api-docs/faqs/spot_glossary

//TODO
//disable_interrupt_coalescing
//disable_gro_lro
//enable RSS
//TODO OPENSSL invece di WOLFSSL

void *cleanup_label = NULL;

int32_t main(void)
{
  keys_t keys;
  event_loop_ctx_t loop;
  WOLFSSL_CTX *ssl_ctx;
  fix_client_t fix_client;
  ws_client_t ws_client;
  rest_client_t rest_client;
  dns_resolver_t dns_resolver;
  cleanup_label = &&cleanup;

  init_logger();
  init_signals();
  init_keys(&keys);
  init_ssl(&ssl_ctx);
  init_fix(&fix_client, &keys, ssl_ctx);
  init_ws(&ws_client, ssl_ctx);
  init_rest(&rest_client, &keys, ssl_ctx);
  init_dns_resolver(&dns_resolver);
  init_event_loop(&loop);

  establish_connections(&loop, &fix_client, &ws_client, &rest_client, &dns_resolver);
  listen_events(&loop, &fix_client, &ws_client, &rest_client);

cleanup:
  free_event_loop(&loop);
  free_dns_resolver(&dns_resolver);
  free_rest(&rest_client);
  free_ws(&ws_client);
  free_fix(&fix_client);
  free_ssl(ssl_ctx);
  free_keys(&keys);
  free_signals();
  free_logger();  
}