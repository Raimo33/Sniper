/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 21:02:36 by craimond          #+#    #+#             */
/*   Updated: 2025/02/09 12:42:57 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clients/fix.h"

COLD static bool send_logon_query(fix_client_t *restrict client);
COLD static bool receive_logon_response(fix_client_t *restrict client);
COLD static bool send_limits_query(fix_client_t *restrict client);
COLD static bool receive_limits_response(fix_client_t *restrict client);

void init_fix(fix_client_t *restrict client, keys_t *restrict keys, SSL_CTX *restrict ssl_ctx)
{
  const uint8_t fd = socket_p(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt_p(fd, IPPROTO_TCP, TCP_FASTOPEN, &(int32_t){5}, sizeof(int32_t));
  setsockopt_p(fd, IPPROTO_TCP, TCP_NODELAY, &(int32_t){1}, sizeof(int32_t));
  setsockopt_p(fd, SOL_SOCKET, SO_KEEPALIVE, &(int32_t){1}, sizeof(int32_t));
  setsockopt_p(fd, IPPROTO_TCP, TCP_KEEPIDLE, &(int32_t){FIX_KEEPALIVE_IDLE}, sizeof(int32_t));
  setsockopt_p(fd, IPPROTO_TCP, TCP_KEEPINTVL, &(int32_t){FIX_KEEPALIVE_INTVL}, sizeof(int32_t));
  setsockopt_p(fd, IPPROTO_TCP, TCP_KEEPCNT, &(int32_t){FIX_KEEPALIVE_CNT}, sizeof(int32_t));
 
  *client = (fix_client_t){
    .sock_fd = fd,
    .addr = {},
    .ssl = init_ssl_socket(fd, ssl_ctx, FIX_HOST),
    .keys = keys,
    .write_buffer = calloc_p(FIX_WRITE_BUFFER_SIZE, sizeof(char)),
    .read_buffer = calloc_p(FIX_READ_BUFFER_SIZE, sizeof(char)),
    .http_response = {},
    .fix_message = {},
    .write_offset = 0,
    .read_offset = 0,
    .msg_seq_num = 1,
  };
}

void handle_fix_connection(UNUSED const uint8_t fd, const uint32_t events, void *data)
{
  static void *restrict states[] = {&&ssl_handshake, &&logon_query, &&logon_response, &&complete};
  static uint8_t sequence = 0;

  fix_client_t *client = data;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic("FIX connection error");

  goto *states[sequence];

ssl_handshake:
  log_msg(STR_AND_LEN("Performing SSL handshake"));
  if (!SSL_connect_p(client->ssl))
    return;
  sequence++;

logon_query:
  log_msg(STR_AND_LEN("Sending logon query"));
  if (!send_logon_query(client))
    return;
  sequence++;

logon_response:
  log_msg(STR_AND_LEN("Receiving logon response"));
  if (!receive_logon_response(client))
    return;
  sequence++;

complete:
  client->status = CONNECTED;
}

void handle_fix_setup(const uint8_t fd, const uint32_t events, void *data)
{
  static void *restrict states[] = {&&limits_query, &&limits_response, &&complete};
  static uint8_t sequence = 0;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic("FIX connection error");

  fix_client_t *client = data;

  goto *states[sequence];

limits_query:
  log_msg(STR_AND_LEN("Sending limits query"));
  if (!send_limits_query(client))
    return;
  sequence++;

limits_response:
  log_msg(STR_AND_LEN("Receiving limits response"));
  if (!receive_limits_response(client)) //TODO salvare i limiti nella struttura client, ogni client ha i suoi limiti
    return;
  sequence++;

  (void)fd;

complete:
  client->status = TRADING;
}

void handle_fix_trading(const uint8_t fd, const uint32_t events, void *data)
{
  //TODO submit di ordini
  (void)fd;
  (void)data;
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
    char seq_num_str[16];
    const uint8_t seq_num_str_len = ultoa(client->msg_seq_num, seq_num_str);

    const fix_message_t raw_data = {
      .fields = {
        {STR_AND_LEN(FIX_MSGTYPE),      STR_AND_LEN(FIX_MSG_TYPE_LOGON)},
        {STR_AND_LEN(FIX_SENDERCOMPID), STR_AND_LEN(FIX_COMP_ID)},
        {STR_AND_LEN(FIX_TARGETCOMPID), STR_AND_LEN("SPOT")},
        {STR_AND_LEN(FIX_MSGSEQNUM),    seq_num_str, seq_num_str_len},
        {STR_AND_LEN(FIX_SENDINGTIME),  timestamp_str, UTC_TIMESTAMP_SIZE},
      },
      .n_fields = 5,
    };

    char serialized_data[1024];
    uint16_t data_len = serialize_fix_message(serialized_data, sizeof(serialized_data), &raw_data);
    
    char signed_data[ED25519_SIGSIZE];
    sign_ed25519(client->keys->priv_key, serialized_data, data_len, signed_data);
    data_len = sizeof(signed_data);
  
    char encoded_data[BASE64_ENCODED_SIZE(ED25519_SIGSIZE) + 1];
    data_len = EVP_EncodeBlock_p((uint8_t *)encoded_data, (const uint8_t *)signed_data, data_len);
  
    char data_len_str[16];
    const uint8_t data_len_str_len = ultoa(data_len, data_len_str);
  
    const fix_message_t message = {
      .fields = {
        {STR_AND_LEN(FIX_MSGTYPE),         STR_AND_LEN(FIX_MSG_TYPE_LOGON)},
        {STR_AND_LEN(FIX_SENDERCOMPID),    STR_AND_LEN(FIX_COMP_ID)},
        {STR_AND_LEN(FIX_TARGETCOMPID),    STR_AND_LEN("SPOT")},
        {STR_AND_LEN(FIX_MSGSEQNUM),       seq_num_str, seq_num_str_len},
        {STR_AND_LEN(FIX_SENDINGTIME),     timestamp_str, UTC_TIMESTAMP_SIZE},
        {STR_AND_LEN(FIX_ENCRYPTMETHOD),   STR_AND_LEN("0")},
        {STR_AND_LEN(FIX_HEARTBTINT),      STR_AND_LEN(FIX_HEARTBEAT_INTERVAL)},
        {STR_AND_LEN(FIX_RAWDATALENGTH),   data_len_str, data_len_str_len},
        {STR_AND_LEN(FIX_RAWDATA),         encoded_data, data_len},
        {STR_AND_LEN(FIX_RESETSEQNUMFLAG), STR_AND_LEN("Y")},
        {STR_AND_LEN(FIX_USERNAME),        (char *)client->keys->api_key, API_KEY_SIZE},
        {STR_AND_LEN(FIX_MESSAGEHANDLING), STR_AND_LEN("1")},
        {STR_AND_LEN(FIX_RESPONSEMODE),    STR_AND_LEN("1")},
      },
      .n_fields = 13,
    };
  
    client->msg_seq_num++;
    len = serialize_fix_message(client->write_buffer, FIX_WRITE_BUFFER_SIZE, &message);
    len = finalize_fix_message(client->write_buffer, FIX_WRITE_BUFFER_SIZE, len);
  }

  return try_ssl_send(client->ssl, client->write_buffer, len, &client->write_offset);
}

static bool receive_logon_response(fix_client_t *restrict client)
{
  if (UNLIKELY(!try_ssl_recv_fix(client->ssl, client->read_buffer, FIX_READ_BUFFER_SIZE, &client->read_offset, &client->fix_message)))
    return false;
  
  //TODO check logon success
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
  close(client->sock_fd);
  
  if (UNLIKELY(client == NULL))
    return;

  free(client->write_buffer);
  free(client->read_buffer);
  free_ssl_socket(client->ssl);
}