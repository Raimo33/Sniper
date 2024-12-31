/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@protonmail.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:07:42 by craimond          #+#    #+#             */
/*   Updated: 2024/12/31 11:40:28 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <curl/curl.h>
#include <libwebsockets.h>
#include <brotli/decode.h>
#include <brotli/encode.h>
#include <uv.h>
#include <jemalloc/jemalloc.h>
#include <yyjson.h>

#define ENDPOINTS_COUNT 6
#define ENDPOINTS_HEALTH_CHECK_INTERVAL 60 * 60 * 1000
#define LOGGING_INTERVAL 60 * 1000
#define MAX_MARKET_ORDER_DELAY 50
#define MAX_LIMIT_ORDER_DELAY 1000

void init_timers(uv_loop_t *loop, char *endpoints[]);

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
  const char *ws_endpoint = "data-stream.binance.vision:9443";

  if (!api_key || !api_secret)
    return (puts_fd("missing api credentials", STDERR_FILENO), 1);
  if (!ca_certs_path)
    return (puts_fd("missing ca certs path", STDERR_FILENO), 1);

  CURL *curl_context = init_rest(ca_certs_path);
  struct lws_context *ws_context = init_ws(ca_certs_path);
  init_fix();

  //TODO healthcheck e ping dei rest endpoint

  struct lws *ws_conn = connect_to_rest(curl_context, rest_endpoints[0]);
    = connect_to_ws(ws_context, ws_endpoint);
    = connect_to_fix();

  uv_loop_t loop;
  uv_loop_init(&loop);
  init_timers(&loop, endpoints);

  uv_run(loop, UV_RUN_DEFAULT);
  //TODO business logic
  uv_stop(loop);
  uv_loop_close(&loop);
  curl_easy_cleanup(&curl);
  curl_global_cleanup();
  return 0;
}

void init_timers(uv_loop_t *loop, char *endpoints[])
{
  uv_timer_t endpoints_timer;
  uv_timer_t logging_timer;

  uv_timer_init(loop, &endpoints_timer);
  uv_timer_init(loop, &logging_timer);

  endpoints_timer.data = endpoints;

  uv_timer_start(&endpoints_timer, update_endpoints, 0, ENDPOINTS_HEALTH_CHECK_INTERVAL);
  uv_timer_start(&logging_timer, log_stats, 0, LOGGING_INTERVAL);
}

//https://developers.binance.com/docs/binance-spot-api-docs/rest-api/account-endpoints
//https://developers.binance.com/docs/binance-spot-api-docs/rest-api/account-endpoints#query-commission-rates-user_data
CURL *init_rest(const char *endpoint, const char *ca_certs_path)  //per settare grandezza ordini (tipo lotti), dal codice si setta solo il profilo di rischio) (per settare le commissioni per rendere i calcoli piu precisi in base alle commissioni dell'account)
{
  if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
    return (puts_fd(curl_easy_strerror(res), STDERR_FILENO), NULL);

  CURL *context = curl_easy_init();
  if (!context)
    return (puts_fd("Failed to initialize curl", STDERR_FILENO), NULL);
  
  //TODO error checking per errori curl_easy_setopt
  curl_easy_setopt(context, CURLOPT_ACCEPT_ENCODING, "br");
  curl_easy_setopt(context, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
  curl_easy_setopt(context, CURLOPT_SSL_VERIFYPEER, 1L);
  curl_easy_setopt(context, CURLOPT_SSL_VERIFYHOST, 2L);
  curl_easy_setopt(context, CURLOPT_CAINFO, ca_certs_path);
  curl_easy_setopt(context, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_3);
  
  return context;
}

//https://developers.binance.com/docs/binance-spot-api-docs/web-socket-streams
struct lws_context *context init_ws(const char *ca_certs_path)//live price data
{
  struct lws_context_creation_info info;
  memset(&info, 0, sizeof(info));
  info.port = CONTEXT_PORT_NO_LISTEN;
  info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
  info.ssl_ca_filepath = ca_certs_path;

  struct lws_context *context = lws_create_context(&info);
  if (!context)
    return (puts_fd("Failed to create websocket context", STDERR_FILENO), NULL);

  return context;
}

//https://developers.binance.com/docs/binance-spot-api-docs/fix-api
void init_fix() //trade spot, submit di ordini, tcp
{
}

void connect_to_rest(CURL *curl, const char *endpoint)
{
  //TODO connessione ai rest endpoint
}

struct lws *connect_to_ws(struct lws_context *context, const char *endpoint)
{
  struct lws_client_connect_info connect_info;
  memset(&connect_info, 0, sizeof(connect_info));
  connect_info.context = context;
  connect_info.address = host;
  connect_info.port = port;
  connect_info.path = "/ws";
  //TODO aggiungere path della stream
  connect_info.ssl_connection = LCCSCF_USE_SSL;
  connect_info.host = lws_canonical_hostname(context);

  struct lws *ws_client = lws_client_connect_via_info(&connect_info);
  if (!ws_client)
    return (puts_fd("Failed to connect to websocket", STDERR_FILENO), 1);

  return ws_client;
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
