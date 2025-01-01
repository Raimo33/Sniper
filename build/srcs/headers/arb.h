/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arb.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@protonmail.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 17:05:09 by craimond          #+#    #+#             */
/*   Updated: 2025/01/01 18:17:06 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARB_H
# define ARB_H

# include <arpa/inet.h>
# include <brotli/decode.h>
# include <brotli/encode.h>
# include <curl/curl.h>
# include <fcntl.h>
# include <jemalloc/jemalloc.h>
# include <libwebsockets.h>
# include <netinet/in.h>
# include <openssl/err.h>
# include <openssl/ssl.h>
# include <uv.h>
# include <yyjson.h>

# include "utils.h"

# define REST_ENDPOINT "api3.binance.com:443"
# define WS_ENDPOINT "data-stream.binance.vision:9443"
# define FIX_ENDPOINT "fix-oe.binance.com:9000"
# define PAIRS = { "usdcust", "fdusdusdt", "usdtdai", "usdpusdt", "fdusdusdc", "eureuri"}
# define LOGGING_INTERVAL 60 * 1000
# define MAX_MARKET_ORDER_DELAY 50
# define MAX_LIMIT_ORDER_DELAY 1000

typedef struct user_data_s {
    
} user_data_t;

typedef struct per_session_data_s {
    
} per_session_data_t;

void init_timers(uv_loop_t *loop);
void init_connection(CURL *curl_context, struct lws *ws_client, const char *api_key, const char *ca_certs_path, uv_loop_t *loop);
void log_stats(uv_timer_t *timer);
void cleanup(CURL *curl_context, struct lws *ws_client, uv_loop_t *loop);

#endif