/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@protonmail.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 17:12:05 by craimond          #+#    #+#             */
/*   Updated: 2025/01/01 18:18:42 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/arb.h"

static CURL       *init_rest(const char *api_key, const char *ca_certs_path, uv_loop_t *loop);
static struct lws *init_ws(const char *api_key, const char *ca_certs_path, uv_loop_t *loop);
static void       init_fix(const char *api_key, const char *ca_certs_path, uv_loop_t *loop);

void init_timers(uv_loop_t *loop)
{
  uv_timer_t logging_timer;

  uv_timer_init(loop, &logging_timer);

  uv_timer_start(&logging_timer, log_stats, 0, LOGGING_INTERVAL);
}

void init_connection(CURL *curl_context, struct lws *wsi, const char *api_key, const char *ca_certs_path, uv_loop_t *loop)
{
  curl_context = init_rest(api_key, ca_certs_path, loop);
  wsi = init_ws(api_key, ca_certs_path, loop);
  //TODO init fix
}

//https://developers.binance.com/docs/binance-spot-api-docs/rest-api/account-endpoints
//https://developers.binance.com/docs/binance-spot-api-docs/rest-api/account-endpoints#query-commission-rates-user_data
static CURL *init_rest(const char *api_key, const char *ca_certs_path, uv_loop_t *loop)  //per settare grandezza ordini (tipo lotti), dal codice si setta solo il profilo di rischio) (per settare le commissioni per rendere i calcoli piu precisi in base alle commissioni dell'account)
{
  if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
    return (err(curl_easy_strerror(res)), NULL);

  CURL *context = curl_easy_init();
  if (!context)
    return (err("Failed to initialize curl"), NULL);
  
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
static struct lws *init_ws(const char *api_key, const char *ca_certs_path, uv_loop_t *loop) //per ricevere dati di mercato in tempo reale
{
  struct lws_protocols *protocols = malloc(2 * sizeof(struct lws_protocols));
  protocols[0] = { "binance-ws", ws_callback, sizeof(per_session_data_t), 1024, 0, NULL, 0 };
  protocols[1] = { NULL, NULL, 0, 0, 0, NULL, 0 };

  const struct lws_context_creation_info info = {
    .port = CONTEXT_PORT_NO_LISTEN,
    .options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT | LWS_SERVER_OPTION_LIBUV,
    .ssl_ca_filepath = ca_certs_path,
    .uv = loop,
    .protocols = protocols,
  }

  struct lws_context *context = lws_create_context(&info);
  if (!context)
    return (err("Failed to create websocket context"), NULL);
  
  user_data_t *user_data = calloc(sizeof(user_data_t));
  char endpoint_copy[sizeof(WS_ENDPOINT)];
  strcpy(endpoint_copy, WS_ENDPOINT);

  const char    *host = strtok(endpoint_copy, ":");
  const uint16_t port = (uint16_t)atoi(strtok(NULL, ":"));
  const char    *path = //TODO merge delle pairs, microsecondtimeunit //https://developers.binance.com/docs/binance-spot-api-docs
  
  const struct lws_retry_bo retry_policy = {
    .secs_since_valid_ping    = 0,
    .secs_since_valid_hangup  = 600,
    .retry_ms_table           = { 1000, 2000, 4000, 8000, 16000, 32000, 64000 },
    .retry_ms_table_count     = 7,
    .conceal_count            = 10,
    .secs_since_valid_rtx     = 180,
    .jitter_percent           = 20,
    .secs_since_valid_stream  = 600
  }
  struct lws *wsi = NULL;
  const struct lws_client_connect_info connect_info = {
    .context                = context,
    .address                = host,
    .host                   = host,
    .port                   = port,
    .path                   = path,
    .ssl_connection         = LCCSCF_USE_SSL,
    .protocol               = protocols[0].name,
    .pwsi                   = &wsi,
    .origin                 = host,
    .userdata               = user_data,
    .retry_and_idle_policy  = &retry_policy,
  }

  //TODO usare api_key

  wsi = lws_client_connect_via_info(&connect_info);
  if (!wsi)
    return (err("Failed to connect to websocket"), NULL);

  return wsi;
}

//https://developers.binance.com/docs/binance-spot-api-docs/fix-api
static void init_fix(const char *api_key, const char *ca_certs_path, uv_loop_t *loop) //submit ordini spot
{
  int16_t sockfd = socket(AF_INET, SOCK_STREAM, 0);
  uint8_t flags = fcntl(sockfd, F_GETFL, 0);
  fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

  char endpoint_copy[sizeof(FIX_ENDPOINT)];
  strcpy(endpoint_copy, endpoint);

  char    *host = strtok(endpoint_copy, ":");
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