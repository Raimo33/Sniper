/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 21:02:36 by craimond          #+#    #+#             */
/*   Updated: 2025/02/04 17:25:50 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/fix.h"

COLD static bool send_logon_query(fix_client_t *restrict client);
COLD static bool receive_logon_response(fix_client_t *restrict client);
COLD static bool send_limits_query(fix_client_t *restrict client);
COLD static bool receive_limits_response(fix_client_t *restrict client);

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

void handle_fix_connection(fix_client_t *restrict client, const uint8_t events, dns_resolver_t *restrict resolver)
{
  static void *restrict states[] = {&&dns_query, &&dns_response, &&connect, &&ssl_handshake, &&logon_query, &&logon_response};

  static uint8_t sequence = 0;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic(STR_AND_LEN("FIX connection error"));

  goto *states[sequence];

dns_query:
  log_msg(STR_AND_LEN("Resolving FIX endpoint: " FIX_HOST));
  resolve_domain(resolver, STR_AND_LEN(FIX_HOST), FIX_FILENO);
  sequence++;
  return;

dns_response:
  log_msg(STR_AND_LEN("Resolved FIX endpoint: " FIX_HOST));
  read(FIX_FILENO, &client->addr.sin_addr.s_addr, sizeof(client->addr.sin_addr.s_addr));
  sequence++;
  return;

connect:
  log_msg(STR_AND_LEN("Connecting to FIX endpoint: " FIX_HOST));
  connect(FIX_FILENO, (struct sockaddr *)&client->addr, sizeof(client->addr));
  sequence++;
  return;

ssl_handshake:
  log_msg(STR_AND_LEN("Performing SSL handshake"));
  sequence += SSL_connect(client->ssl) == true;
  return;

logon_query:
  log_msg(STR_AND_LEN("Sending logon query"));
  sequence += send_logon_query(client);
  return;

logon_response:
  log_msg(STR_AND_LEN("Receiving logon response"));
  client->connected = receive_logon_response(client);
}

//TODO capire il valore di return
void handle_fix_setup(fix_client_t *restrict client, const uint8_t events, graph_t *restrict graph)
{
  static void *restrict states[] = {&&limits_query, &&limits_response};
  static uint8_t sequence = 0;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic(STR_AND_LEN("FIX connection error"));

  goto *states[sequence];

limits_query:
  log_msg(STR_AND_LEN("Sending limits query"));
  sequence += send_limits_query(client);
  return;

limits_response:
  log_msg(STR_AND_LEN("Receiving limits response"));
  receive_limits_response(client);

  (void)graph;
}

void handle_fix_trading(fix_client_t *restrict client, const uint8_t events, graph_t *restrict graph)
{
  //TODO submit di ordini
  (void)client;
  (void)graph;
  (void)events;
  return;
}

static bool send_logon_query(fix_client_t *restrict client)
{
  static bool initialized;
  static uint16_t len;

  if (!initialized)
  {
    const char *timestamp_str = get_timestamp_utc_str();

    const fix_field_t raw_data[] = {
      {STR_AND_LEN(FIX_MSGTYPE),      STR_AND_LEN(FIX_MSG_TYPE_LOGON)},
      {STR_AND_LEN(FIX_SENDERCOMPID), STR_AND_LEN(FIX_COMP_ID)},
      {STR_AND_LEN(FIX_TARGETCOMPID), STR_AND_LEN("SPOT")},
      {STR_AND_LEN(FIX_MSGSEQNUM),    client->msg_seq_num, xx}, //TODO mettere a stringa
      {STR_AND_LEN(FIX_SENDINGTIME),  timestamp_str, UTC_TIMESTAMP_SIZE},
    };
    char raw_data_str[1024];
    const uint16_t raw_data_len = serialize_fix_fields(raw_data_str, sizeof(raw_data_str), raw_data, ARR_LEN(raw_data));
    const fix_field_t fields[] = {
      {STR_AND_LEN(FIX_BEGINSTRING),     STR_AND_LEN(FIX_VERSION)},
      {STR_AND_LEN(FIX_BODYLENGTH),      NULL, 0}, //TODO se il serializer lo trova, lo riempie
      {STR_AND_LEN(FIX_MSGTYPE),         STR_AND_LEN(FIX_MSG_TYPE_LOGON)},
      {STR_AND_LEN(FIX_SENDERCOMPID),    STR_AND_LEN(FIX_COMP_ID)},
      {STR_AND_LEN(FIX_TARGETCOMPID),    STR_AND_LEN("SPOT")},
      {STR_AND_LEN(FIX_MSGSEQNUM),       client->msg_seq_num, xx},
      {STR_AND_LEN(FIX_SENDINGTIME),     timestamp_str, xx},
      {STR_AND_LEN(FIX_ENCRYPTMETHOD),   STR_AND_LEN("0")},
      {STR_AND_LEN(FIX_HEARTBTINT),      STR_AND_LEN(FIX_HEARTBEAT_INTERVAL)},
      {STR_AND_LEN(FIX_RAWDATALENGTH),   0, xx},
      {STR_AND_LEN(FIX_RAWDATA),         raw_data_str, raw_data_len},
      {STR_AND_LEN(FIX_RESETSEQNUMFLAG), STR_AND_LEN("Y")},
      {STR_AND_LEN(FIX_USERNAME),        (char *)client->keys->api_key, API_KEY_SIZE},
      {STR_AND_LEN(FIX_MESSAGEHANDLING), STR_AND_LEN("1")},
      {STR_AND_LEN(FIX_RESPONSEMODE),    STR_AND_LEN("1")},
      {STR_AND_LEN(FIX_CHECKSUM),        NULL, 0} //TODO se il serializer lo trova, lo riempie
    };
  
    client->msg_seq_num++;
    len = serialize_fix_fields(client->write_buffer, FIX_WRITE_BUFFER_SIZE, fields, ARR_LEN(fields));
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
  close(FIX_FILENO);
  
  if (UNLIKELY(client == NULL))
    return;

  free(client->write_buffer);
  free(client->read_buffer);
  free_ssl_socket(client->ssl);
}