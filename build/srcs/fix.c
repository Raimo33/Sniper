/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 21:02:36 by craimond          #+#    #+#             */
/*   Updated: 2025/02/03 12:54:23 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/fix.h"

COLD static bool send_logon_query(fix_client_t *restrict client);
COLD static bool receive_logon_response(fix_client_t *restrict client);
COLD static bool send_limits_query(fix_client_t *restrict client);
COLD static bool receive_limits_response(fix_client_t *restrict client);
HOT static void format_price(const fixed_point_t price, char *buffer); //TODO 5. Zero-Cost String Formatting, Precomputed FIX template (e.g., "44=XXXX|")

void init_fix(fix_client_t *restrict client, const keys_t *restrict keys, SSL_CTX *restrict ssl_ctx)
{
  const uint16_t fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt(fd, IPPROTO_TCP, TCP_FASTOPEN, &(bool){true}, sizeof(bool));
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(bool){true}, sizeof(bool));
  setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &(bool){true}, sizeof(bool));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &(uint16_t){FIX_KEEPALIVE_IDLE}, sizeof(uint16_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &(uint16_t){FIX_KEEPALIVE_INTVL}, sizeof(uint16_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &(uint16_t){FIX_KEEPALIVE_CNT}, sizeof(uint16_t));
 
  *client = (fix_client_t){
    .addr = {
      .sin_family = AF_INET,
      .sin_port = htons(FIX_PORT),
      .sin_addr = {
        .s_addr = INADDR_NONE
      }
    },
    .ssl = init_ssl_socket(fd, ssl_ctx),
    .keys = keys,
    .write_buffer = calloc(FIX_WRITE_BUFFER_SIZE, sizeof(char)),
    .read_buffer = calloc(FIX_READ_BUFFER_SIZE, sizeof(char)),
    .write_offset = 0,
    .read_offset = 0,
    .msg_seq_num = 0
  };

  dup2(fd, FIX_FILENO);
  close(fd);
}

bool handle_fix_connection(fix_client_t *restrict client, const uint8_t events, dns_resolver_t *restrict resolver)
{
  static void *restrict states[] = {&&dns_query, &&dns_response, &&connect, &&ssl_handshake, &&logon_query, &&logon_response};

  static uint8_t sequence = 0;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic(STR_LEN_PAIR("FIX connection error"));

  goto *states[sequence];

dns_query:
  log_msg(STR_LEN_PAIR("Resolving FIX endpoint: " FIX_HOST));
  resolve_domain(resolver, STR_LEN_PAIR(FIX_HOST), FIX_FILENO);
  sequence++;
  return false;

dns_response:
  log_msg(STR_LEN_PAIR("Resolved FIX endpoint: " FIX_HOST));
  read(FIX_FILENO, &client->addr.sin_addr.s_addr, sizeof(client->addr.sin_addr.s_addr));
  sequence++;
  return false;

connect:
  log_msg(STR_LEN_PAIR("Connecting to FIX endpoint: " FIX_HOST));
  connect(FIX_FILENO, (struct sockaddr *)&client->addr, sizeof(client->addr));
  sequence++;
  return false;

ssl_handshake:
  log_msg(STR_LEN_PAIR("Performing SSL handshake"));
  sequence += SSL_connect(client->ssl) == true;
  return false;

logon_query:
  log_msg(STR_LEN_PAIR("Sending logon query"));
  sequence += send_logon_query(client);
  return false;

logon_response:
  log_msg(STR_LEN_PAIR("Receiving logon response"));
  sequence += receive_logon_response(client);
  return false;
}

bool handle_fix_setup(fix_client_t *restrict client, graph_t *restrict graph)
{
  static void *restrict states[] = {&&limits_query, &&limits_response};
  static uint8_t sequence = 0;

  goto *states[sequence];

limits_query:
  log_msg(STR_LEN_PAIR("Sending limits query"));
  sequence += send_limits_query(client);
  return false;

limits_response:
  log_msg(STR_LEN_PAIR("Receiving limits response"));
  return receive_limits_response(client);

  (void)graph;
}

bool handle_fix_trading(fix_client_t *restrict client, graph_t *restrict graph)
{
  //TODO submit di ordini
  (void)client;
  (void)graph;
  return false;
}

static bool send_logon_query(fix_client_t *restrict client)
{
  //TODO generare payload
  static bool initialized;
  static uint16_t len;

  if (!initialized)
  {
  //   fix_message_t message = {
      
  //   }
  //   len = build_fix_message(client->write_buffer, FIX_WRITE_BUFFER_SIZE);
    initialized = true;
  }

  return try_ssl_send(client->ssl, client->write_buffer, len, &client->write_offset);
}

static bool receive_logon_response(fix_client_t *restrict client)
{
  //TODO
  (void)client;
  return false;
}

static bool send_limits_query(fix_client_t *restrict client)
{
  //TODO
  (void)client;
  return false;
}

static bool receive_limits_response(fix_client_t *restrict client)
{
  //TODO
  (void)client;
  return false;
}

UNUSED static void format_price(const fixed_point_t price, char *buffer)
{
  //TODO
  (void)price;
  (void)buffer;
}

void free_fix(fix_client_t *restrict client)
{
  free(client->write_buffer);
  free(client->read_buffer);
  free_ssl_socket(client->ssl);
  close(FIX_FILENO);
}