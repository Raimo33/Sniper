/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@protonmail.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:07:42 by craimond          #+#    #+#             */
/*   Updated: 2024/12/31 15:57:46 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//https://developers.binance.com/docs/binance-spot-api-docs/faqs/spot_glossary

#include <curl/curl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <libwebsockets.h>
#include <brotli/decode.h>
#include <brotli/encode.h>
#include <uv.h>
#include <jemalloc/jemalloc.h>
#include <yyjson.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define REST_ENDPOINT "https://api3.binance.com:443"
#define WS_ENDPOINT "wss://data-stream.binance.vision:9443"
#define FIX_ENDPOINT "tcp+tls://fix-oe.binance.com:9000"
#define PAIRS = { "usdcust", "fdusdusdt", "usdtdai", "usdpusdt", "fdusdusdc", "eureuri"}
#define LOGGING_INTERVAL 60 * 1000
#define MAX_MARKET_ORDER_DELAY 50
#define MAX_LIMIT_ORDER_DELAY 1000

//TODO error checking per tutte le funzioni

int32_t main(void)
{
  const char *api_key = getenv("API_KEY");
  const char *ca_certs_path = getenv("CA_CERTS_PATH");

  if (!api_key)
    return (puts_fd("missing api key", STDERR_FILENO), 1);
  if (!ca_certs_path)
    return (puts_fd("missing ca certs path", STDERR_FILENO), 1);

  uv_loop_t loop;
  uv_loop_init(&loop);
  init_timers(&loop, endpoints);

  CURL *curl_context = init_rest(api_key, ca_certs_path, &loop);
  struct lws *ws_client = init_ws(api_key, ca_certs_path, &loop);
    = init_fix(api_key, ca_certs_path, &loop);

  uv_run(loop, UV_RUN_DEFAULT);
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
CURL *init_rest(const char *api_key, const char *ca_certs_path, uv_loop_t *loop)  //per settare grandezza ordini (tipo lotti), dal codice si setta solo il profilo di rischio) (per settare le commissioni per rendere i calcoli piu precisi in base alle commissioni dell'account)
{
  if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
    return (puts_fd(curl_easy_strerror(res), STDERR_FILENO), NULL);

  CURL *context = curl_easy_init();
  if (!context)
    return (puts_fd("Failed to initialize curl", STDERR_FILENO), NULL);
  
  curl_easy_setopt(context, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
  curl_easy_setopt(context, CURLOPT_URL, REST_ENDPOINT);
  curl_easy_setopt(context, CURLOPT_SSL_VERIFYPEER, 1L);
  curl_easy_setopt(context, CURLOPT_SSL_VERIFYHOST, 2L);
  curl_easy_setopt(context, CURLOPT_ACCEPT_ENCODING, "br");
  curl_easy_setopt(context, CURLOPT_CAINFO, ca_certs_path);
  curl_easy_setopt(context, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_3);
  
  //TODO usare api_key e loop async uv
  //TODO curl async con CURLM *multi_handle;

  return context;
}

//https://developers.binance.com/docs/binance-spot-api-docs/web-socket-streams#individual-symbol-book-ticker-streams
struct lws *init_ws(const char *api_key, const char *ca_certs_path, uv_loop_t *loop) //per ricevere dati di mercato in tempo reale
{
  struct lws_context_creation_info info;
  memset(&info, 0, sizeof(info));
  info.port = CONTEXT_PORT_NO_LISTEN;
  info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT | LWS_SERVER_OPTION_LIBUV;
  info.ssl_ca_filepath = ca_certs_path;
  info.uv = loop;

  const struct lws_context *context = lws_create_context(&info);
  if (!context)
    return (puts_fd("Failed to create websocket context", STDERR_FILENO), NULL);

  char endpoint_copy[sizeof(WS_ENDPOINT)];
  strcpy(endpoint_copy, endpoint);

  const char *host = strtok(endpoint_copy, ":");
  const uint16_t port = (uint16_t)atoi(strtok(NULL, ":"));
  const char *path = //TODO merge delle pairs, microsecondtimeunit //https://developers.binance.com/docs/binance-spot-api-docs

  struct lws_client_connect_info connect_info;
  memset(&connect_info, 0, sizeof(connect_info));
  connect_info.context = context;
  connect_info.address = host;
  connect_info.port = port;
  connect_info.path = path;
  connect_info.ssl_connection = LCCSCF_USE_SSL;
  connect_info.host = lws_canonical_hostname(context);

  //TODO usare api_key

  struct lws *ws_client = lws_client_connect_via_info(&connect_info);
  if (!ws_client)
    return (puts_fd("Failed to connect to websocket", STDERR_FILENO), 1);

  return ws_client;
}

//https://developers.binance.com/docs/binance-spot-api-docs/fix-api
void init_fix(const char *api_key, const char *ca_certs_path, uv_loop_t *loop) //submit ordini spot
{
  int16_t sockfd = socket(AF_INET, SOCK_STREAM, 0);
  uint8_t flags = fcntl(sockfd, F_GETFL, 0);
  fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

  char endpoint_copy[sizeof(FIX_ENDPOINT)];
  strcpy(endpoint_copy, endpoint);

  char *host = strtok(endpoint_copy, ":");
  uint16_t port = (uint16_t)atoi(strtok(NULL, ":"));

  struct sockaddr_in server_addr = {0};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  inet_pton(AF_INET, host, &server_addr.sin_addr);

  connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

  SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
  SSL_CTX_load_verify_locations(ctx, ca_certs_path, NULL);
  SSL *ssl = SSL_new(ctx);
  SSL_set_fd(ssl, sockfd);
 
  //TODO usare api_key
  //TODO usare uv_poll_t per gestire la connessione
}

void update_endpoints(uv_timer_t *timer)
{
  char **endpoints = (char **)timer->data;
  for (uint8_t i = 0; i < REST_ENDPOINTS_COUNT; i++)
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
