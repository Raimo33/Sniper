/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@protonmail.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:07:42 by craimond          #+#    #+#             */
/*   Updated: 2024/12/31 01:19:05 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <curl/curl.h>
#include <libwebsockets.h>
#include <brotli/decode.h>
#include <brotli/encode.h>
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
  const char *ca_certs_path = getenv("CA_CERTS_PATH");
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
    return (puts_fd("API_KEY or API_SECRET not set", STDERR_FILENO), 1);
  
  CURL curl;
  uv_loop_t loop;

  uv_loop_init(&loop);
  setup_timers(&loop, endpoints);
  setup_rest(&curl, ca_certs_path);
  setup_ws();
  setup_fix();
  uv_run(loop, UV_RUN_DEFAULT);
  uv_stop(loop);
  uv_loop_close(&loop);
  curl_easy_cleanup(&curl);
  curl_global_cleanup();
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

void setup_rest(CURL *curl, const char *ca_certs_path)
{
  if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
    return (puts_fd(curl_easy_strerror(res), STDERR_FILENO), 1);

  curl = curl_easy_init();
  if (!curl)
    return (puts_fd("Failed to initialize curl", STDERR_FILENO), 1);
  
  curl_version_info_data *info = curl_version_info(CURLVERSION_NOW);
  if (info->features & CURL_VERSION_BROTLI)
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "br");

  curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, 
    (info->features & CURL_VERSION_HTTP2) ? CURL_HTTP_VERSION_2_0 : CURL_HTTP_VERSION_1_1);

  if (info->features & CURL_VERSION_SSL) #TODO error checking per curl_easy_setopt
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    curl_easy_setopt(curl, CURLOPT_CAINFO, ca_certs_path);
    curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_3);
  else
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    
  //https://developers.binance.com/docs/binance-spot-api-docs/rest-api/account-endpoints (#TODO per settare grandezza ordini (tipo lotti), dal codice si setta solo il profilo di rischio)
  //https://developers.binance.com/docs/binance-spot-api-docs/rest-api/account-endpoints#query-commission-rates-user_data (per settare le commissioni per rendere i calcoli piu precisi in base alle commissioni dell'account)
}

void setup_ws(void) //TODO live price data
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
