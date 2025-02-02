/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:53:55 by craimond          #+#    #+#             */
/*   Updated: 2025/02/02 12:28:42 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/rest.h"

COLD static bool send_info_query(rest_client_t *restrict client);
COLD static bool receive_info_response(rest_client_t *restrict client);
COLD static void parse_info_response(char *restrict input, const uint32_t input_len);
COLD static void process_info_chunks(const uint16_t read_fd);

void init_rest(rest_client_t *restrict client, const keys_t *restrict keys, SSL_CTX *restrict ssl_ctx)
{
  client->addr = (struct sockaddr_in){
    .sin_family = AF_INET,
    .sin_port = htons(REST_PORT),
    .sin_addr = {
      .s_addr = INADDR_NONE
    }
  };
  client->keys = keys;
  client->write_buffer = calloc(REST_WRITE_BUFFER_SIZE, sizeof(char));
  client->read_buffer = calloc(REST_READ_BUFFER_SIZE, sizeof(char));

  const uint16_t fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt(fd, IPPROTO_TCP, TCP_FASTOPEN, &(bool){true}, sizeof(bool));
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(bool){true}, sizeof(bool));
  setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &(bool){true}, sizeof(bool));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &(uint16_t){REST_KEEPALIVE_IDLE}, sizeof(uint16_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &(uint16_t){REST_KEEPALIVE_INTVL}, sizeof(uint16_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &(uint16_t){REST_KEEPALIVE_CNT}, sizeof(uint16_t));
  client->ssl = init_ssl_socket(fd, ssl_ctx);

  dup2(fd, REST_FILENO);
  close(fd);
}

bool handle_rest_connection(rest_client_t *restrict client, const uint8_t events, dns_resolver_t *restrict resolver)
{
  static void *restrict states[] = {&&dns_query, &&dns_response, &&connect, &&ssl_handshake};
  static uint8_t sequence = 0;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic(STR_LEN_PAIR("REST connection error"));

  goto *states[sequence];

dns_query:
  log_msg(STR_LEN_PAIR("Resolving REST endpoint: " REST_HOST));
  resolve_domain(resolver, STR_LEN_PAIR(REST_HOST), REST_FILENO);
  sequence++;
  return false;

dns_response:
  log_msg(STR_LEN_PAIR("Resolved REST endpoint: " REST_HOST));
  read(REST_FILENO, &client->addr.sin_addr.s_addr, sizeof(client->addr.sin_addr.s_addr));
  sequence++;
  return false;

connect:
  log_msg(STR_LEN_PAIR("Connecting to REST endpoint: " REST_HOST));
  connect(REST_FILENO, (struct sockaddr *)&client->addr, sizeof(client->addr));
  sequence++;
  return false;

ssl_handshake:
  log_msg(STR_LEN_PAIR("Performing SSL handshake"));
  return SSL_connect(client->ssl) == true;
}

bool handle_rest_setup(rest_client_t *restrict client, graph_t *restrict graph)
{
  static void *restrict states[] = {&&info_query, &&info_response};
  static uint8_t sequence = 0;

  goto *states[sequence];

info_query:
  log_msg(STR_LEN_PAIR("Querying Exchange info"));
  sequence += send_info_query(client);
  return false;

info_response:
  log_msg(STR_LEN_PAIR("Received Exchange info"));
  return receive_info_response(client);

  (void)graph;
  //TODO altre chiamate per user data, limiti account ecc
}

bool handle_rest_trading(rest_client_t *restrict client, graph_t *restrict graph)
{
  //TODO eventuali chiamate di rest durante il trading
  (void)client;
  (void)graph;
  return false;
}

static bool send_info_query(rest_client_t *restrict client)
{
  static const char query[] = 
    "GET /api/v3/exchangeInfo?permissions=SPOT&showPermissionSets=false&symbolStatus=TRADING HTTP/1.1\r"
    "Host: " REST_HOST "\r\n"
    "Accept-Encoding: gzip\r\n"
    "Connection: keep-alive\r\n"
    "\r\n";
  static bool first = true;

  if (first)
  {
    memcpy(client->write_buffer, query, sizeof(query));
    first = false;
  }

  return try_ssl_send(client->ssl, client->write_buffer, sizeof(query), &client->write_offset);
}

static bool receive_info_response(rest_client_t *restrict client)
{
  static const uint32_t info_response_size = 262144 * sizeof(char);
  static bool initialized = false;

  if (!initialized)
  {
    client->read_buffer = realloc(client->read_buffer, info_response_size);
    initialized = true;
  }

  if (UNLIKELY(try_ssl_recv_http(client->ssl, client->read_buffer, info_response_size, &client->read_offset, &client->http_response) == false))
    return false;

  const http_response_t *restrict response = &client->http_response;
  fast_assert(response->status_code == 200, STR_LEN_PAIR("Exchange info query failed: invalid status code"));
  
  const header_entry_t *restrict content_encoding = header_map_get(&response->headers, STR_LEN_PAIR("content-encoding"));
  fast_assert(content_encoding, STR_LEN_PAIR("Exchange info query failed: missing content encoding header"));
  fast_assert(memcmp(content_encoding->value, STR_LEN_PAIR("gzip")) == 0, STR_LEN_PAIR("Exchange info query failed: invalid content encoding"));
  
  parse_info_response(client->read_buffer, client->read_offset);

  free_http_response(&client->http_response);
  client->read_buffer = realloc(client->read_buffer, REST_READ_BUFFER_SIZE);
  return true;
}

static void parse_info_response(char *restrict input, const uint32_t input_len)
{
  int32_t pipe_fds[2];
  pipe(pipe_fds);
  const int32_t pid = fork();

  if (pid == 0)
  {
    close(pipe_fds[0]);
    gzip_decompress_to_file((uint8_t *)input, input_len, pipe_fds[1]);
    exit(EXIT_SUCCESS);
  }
  else
  {
    int32_t status;
    close(pipe_fds[1]);
    process_info_chunks(pipe_fds[0]);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
    {
      int8_t exit_code = WEXITSTATUS(status);
      if (exit_code != EXIT_SUCCESS)
        exit(exit_code);
    }
    else
      panic(STR_LEN_PAIR("Failed to decompress exchange info response"));
  } 
}

static void process_info_chunks(const uint16_t read_fd)
{
  UNUSED uint8_t chunk[PIPE_BUF_SIZE];
  UNUSED int32_t read_bytes;

  //https://ibireme.github.io/yyjson/doc/doxygen/html/
  //TODO json parser?
  //TODO process the info response by filling the trading pairs graph

  close(read_fd);
}

void free_rest(rest_client_t *restrict rest)
{
  free(rest->write_buffer);
  free(rest->read_buffer);
  free_ssl_socket(rest->ssl);
  close(REST_FILENO);
}