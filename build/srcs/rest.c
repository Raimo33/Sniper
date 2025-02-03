/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:53:55 by craimond          #+#    #+#             */
/*   Updated: 2025/02/03 22:54:28 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/rest.h"

COLD static bool send_info_query(rest_client_t *restrict client);
COLD static bool receive_info_response(rest_client_t *restrict client);
COLD static void process_info_response(char *body, const uint32_t body_len);

void init_rest(rest_client_t *restrict client, const keys_t *restrict keys, SSL_CTX *restrict ssl_ctx)
{

  const uint16_t fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt(fd, IPPROTO_TCP, TCP_FASTOPEN, &(bool){true}, sizeof(bool));
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(bool){true}, sizeof(bool));
  setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &(bool){true}, sizeof(bool));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &(uint16_t){REST_KEEPALIVE_IDLE}, sizeof(uint16_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &(uint16_t){REST_KEEPALIVE_INTVL}, sizeof(uint16_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &(uint16_t){REST_KEEPALIVE_CNT}, sizeof(uint16_t));

  *client = (rest_client_t){
    .addr = {
      .sin_family = AF_INET,
      .sin_port = htons(REST_PORT),
      .sin_addr = {
        .s_addr = INADDR_NONE
      }
    },
    .ssl = init_ssl_socket(fd, ssl_ctx),
    .keys = keys,
    .write_buffer = calloc(REST_WRITE_BUFFER_SIZE, sizeof(char)),
    .read_buffer = calloc(REST_READ_BUFFER_SIZE, sizeof(char)),
    .http_response = {},
    .write_offset = 0,
    .read_offset = 0
  };

  dup2(fd, REST_FILENO);
  close(fd);
}

void handle_rest_connection(rest_client_t *restrict client, const uint8_t events, dns_resolver_t *restrict resolver)
{
  static void *restrict states[] = {&&dns_query, &&dns_response, &&connect, &&ssl_handshake};
  static uint8_t sequence = 0;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic(STR_AND_LEN("REST connection error"));

  goto *states[sequence];

dns_query:
  log_msg(STR_AND_LEN("Resolving REST endpoint: " REST_HOST));
  resolve_domain(resolver, STR_AND_LEN(REST_HOST), REST_FILENO);
  sequence++;
  return;

dns_response:
  log_msg(STR_AND_LEN("Resolved REST endpoint: " REST_HOST));
  read(REST_FILENO, &client->addr.sin_addr.s_addr, sizeof(client->addr.sin_addr.s_addr));
  sequence++;
  return;

connect:
  log_msg(STR_AND_LEN("Connecting to REST endpoint: " REST_HOST));
  connect(REST_FILENO, (struct sockaddr *)&client->addr, sizeof(client->addr));
  sequence++;
  return;

ssl_handshake:
  log_msg(STR_AND_LEN("Performing SSL handshake"));
  client->connected = SSL_connect(client->ssl);
}

//TODO capire return
void handle_rest_setup(rest_client_t *restrict client, const uint8_t events, graph_t *restrict graph)
{
  static void *restrict states[] = {&&info_query, &&info_response};
  static uint8_t sequence = 0;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic(STR_AND_LEN("REST connection error"));

  goto *states[sequence];

info_query:
  log_msg(STR_AND_LEN("Querying Exchange info"));
  sequence += send_info_query(client);
  return;

info_response:
  log_msg(STR_AND_LEN("Received Exchange info"));
  receive_info_response(client);

  (void)graph;
  //TODO altre chiamate per user data, limiti account ecc
}

void handle_rest_trading(rest_client_t *restrict client, const uint8_t events, graph_t *restrict graph)
{
  //TODO eventuali chiamate di rest durante il trading
  (void)client;
  (void)graph;
  (void)events;
}

static bool send_info_query(rest_client_t *restrict client)
{
  static bool initialized;
  static uint16_t len;

  if (!initialized)
  {
    const http_request_t request = {
      .method = GET,
      .path = "/api/v3/exchangeInfo?permissions=SPOT&showPermissionSets=false&symbolStatus=TRADING",
      .path_len = STR_LEN("/api/v3/exchangeInfo?permissions=SPOT&showPermissionSets=false&symbolStatus=TRADING"),
      .version = HTTP_1_1,
      .headers = (header_entry_t[]){
        { STR_AND_LEN("Host"), STR_AND_LEN(REST_HOST) },
        { STR_AND_LEN("Accept-Encoding"), STR_AND_LEN("gzip") },
        { STR_AND_LEN("Connection"), STR_AND_LEN("keep-alive") }
      },
      .n_headers = 3,
      .body = NULL,
      .body_len = 0
    };
    
    len = serialize_http_request(client->write_buffer, REST_WRITE_BUFFER_SIZE, &request);
    initialized = true;
  }

  return try_ssl_send(client->ssl, client->write_buffer, len, &client->write_offset);
}

static bool receive_info_response(rest_client_t *restrict client)
{
  static const uint32_t info_response_size = 262144 * sizeof(char);
  static bool initialized;

  if (!initialized)
  {
    client->read_buffer = realloc(client->read_buffer, info_response_size);
    initialized = true;
  }

  if (UNLIKELY(try_ssl_recv_http(client->ssl, client->read_buffer, info_response_size, &client->read_offset, &client->http_response) == false))
    return false;

  const http_response_t *restrict response = &client->http_response;
  fast_assert(response->status_code == 200, "Exchange info query failed: invalid status code");
  
  const header_entry_t *restrict content_encoding = header_map_get(&response->headers, STR_AND_LEN("content-encoding"));
  fast_assert(content_encoding, "Exchange info query failed: missing content encoding header");
  fast_assert(memcmp(content_encoding->value, STR_AND_LEN("gzip")) == 0, "Exchange info query failed: invalid content encoding");
  fast_assert(response->body, "Exchange info query failed: missing body");
  
  process_info_response(response->body, response->body_len);

  free_http_response(&client->http_response);
  client->read_buffer = realloc(client->read_buffer, REST_READ_BUFFER_SIZE);
  return true;
}

static void process_info_response(char *body, const uint32_t body_len)
{
  int32_t pipe_fds[2];
  pipe(pipe_fds);

  const pid_t pid = fork();
  if (pid == 0)
  {
    close(pipe_fds[0]);
    gzip_decompress_to_file((uint8_t *)body, body_len, (uint16_t)pipe_fds[1]);
    close(pipe_fds[1]);
    exit(EXIT_SUCCESS);
  }
  else
  {
    close(pipe_fds[1]);
    FILE *restrict fp = fdopen(pipe_fds[0], "rb");
    yyjson_doc *restrict doc = yyjson_read_fp(fp, 0, NULL, NULL);
    fclose(fp);
    //TODO processare il json
    yyjson_doc_free(doc);
    wait_child_process(pid);
  }
}

void free_rest(rest_client_t *restrict rest)
{
  close(REST_FILENO);
  
  if (UNLIKELY(rest == NULL))
    return;

  free(rest->write_buffer);
  free(rest->read_buffer);
  free_ssl_socket(rest->ssl);
}