/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@protonmail.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:07:42 by craimond          #+#    #+#             */
/*   Updated: 2024/12/30 16:57:07 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <curl/curl.h>
#include <nghttp2/nghttp2.h>
#include <ngtcp2/ngtcp2.h>
#include <libwebsockets.h>
#include <brotli/decode.h>
#include <brotli/encode.h>
#include <zlib.h>
#include <zstd.h>
#include <uv.h>
#include <openssl/ssl.h>
#include <jemalloc/jemalloc.h>
#include <yyjson.h>

#define ENDPOINTS_COUNT 6
#define ENDPOINTS_HEALTH_CHECK_INTERVAL 60 * 60 * 1000
#define LOGGING_INTERVAL 60 * 1000
#define MAX_MARKET_ORDER_DELAY 50
#define MAX_LIMIT_ORDER_DELAY 1000

void setup_timers(uv_loop_t *loop, char *endpoints[]);

int main(void)
{
  const char *api_key = getenv("API_KEY");
  const char *api_secret = getenv("API_SECRET");
  char *rest_endpoints[] = {
    "https://api.binance.com",
    "https://api-gcp.binance.com",
    "https://api1.binance.com",
    "https://api2.binance.com",
    "https://api3.binance.com",
    "https://api4.binance.com"
  };
  const char *fix_order_entry_endpoint = "fix-oe.binance.com:9000";
  const char *fix_drop_copy_endpoint = "fix-dc.binance.com:9000";
  const char *ws_endpoint = "data-stream.binance.vision:443";

  if (!api_key || !api_secret)
    panic("Missing API credentials in env");

  uv_loop_t loop;
  uv_loop_init(&loop);
  setup_timers(&loop, endpoints);
  setup_rest();
  setup_ws();
  setup_fix();
  uv_run(loop, UV_RUN_DEFAULT);
  uv_stop(loop);
  uv_loop_close(&loop);

  return 0;
}

void setup_timers(uv_loop_t *loop, char *endpoints[])
{
  uv_timer_t endpoints_timer;
  uv_timer_t logging_timer;

  uv_timer_init(loop, &endpoints_timer);
  uv_timer_init(loop, &logging_timer);

  endpoints_timer.data = endpoints;

  uv_timer_start(&endpoints_timer, update_endpoints, 0, ENDPOINTS_HEALTH_CHECK_INTERVAL);
  uv_timer_start(&logging_timer, log_stats, 0, LOGGING_INTERVAL);
}

void setup_rest() //TODO informazioni generiche di inizializzazione
{
  //https://developers.binance.com/docs/binance-spot-api-docs/rest-api/account-endpoints (#TODO per settare grandezza ordini (tipo lotti), dal codice si setta solo il profilo di rischio)
  //https://developers.binance.com/docs/binance-spot-api-docs/rest-api/account-endpoints#query-commission-rates-user_data (per settare le commissioni per rendere i calcoli piu precisi in base alle commissioni dell'account)
}

void setup_ws() //TODO live price data
{
  https://developers.binance.com/docs/binance-spot-api-docs/web-socket-streams
}

void setup_fix() //TODO trade spot, submit di ordini, tcp
{
  //https://developers.binance.com/docs/binance-spot-api-docs/fix-api
}

void update_endpoints(uv_timer_t *timer)
{
  char **endpoints = (char **)timer->data;
  for (int i = 0; i < ENDPOINTS_COUNT; i++)
  {
    //TODO check endpoint health
    //https://developers.binance.com/docs/binance-spot-api-docs/rest-api/data-sources
  }
}

uint8_t get_health_score(const char *endpoint)
{
  //TODO algoritmo con vari pesi per misurare la salute di un endpoint
  //https://developers.binance.com/docs/binance-spot-api-docs/rest-api/general-endpoints#test-connectivity
}
