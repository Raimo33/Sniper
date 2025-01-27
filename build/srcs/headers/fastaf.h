/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fastaf.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 14:15:47 by craimond          #+#    #+#             */
/*   Updated: 2025/01/27 14:21:04 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FASTAF_H
# define FASTAF_H

# include "logger.h"
# include "errors.h"
# include "signals.h"
# include "fix.h"
# include "ws.h"
# include "rest.h"
# include "dns_resolver.h"
# include "event_loop.h"
# include "keys.h"
# include "ssl.h"

typedef struct
{
  keys_t keys;
  event_loop_ctx_t loop;
  SSL_CTX *ssl_ctx;
  fix_client_t fix_client;
  ws_client_t ws_client;
  rest_client_t rest_client;
  dns_resolver_t dns_resolver;
} app_resources_t;

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

#endif