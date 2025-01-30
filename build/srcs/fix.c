/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 21:02:36 by craimond          #+#    #+#             */
/*   Updated: 2025/01/30 18:27:40 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/fix.h"

static bool COLD send_logon(const fix_client_t *restrict client);
static bool COLD receive_logon(const fix_client_t *restrict client);
static bool COLD send_limit_query(const fix_client_t *restrict client);
static bool COLD receive_limit_query(const fix_client_t *restrict client);
static void HOT format_price(const fixed_point_t price, char *buffer); //TODO 5. Zero-Cost String Formatting, Precomputed FIX template (e.g., "44=XXXX|")

//TODO pool di connessioni
void init_fix(fix_client_t *restrict client, const keys_t *restrict keys, const SSL_CTX *restrict ssl_ctx)
{
  client->addr = (struct sockaddr_in){
    .sin_family = AF_INET,
    .sin_port = htons(FIX_PORT),
    .sin_addr = {
      .s_addr = INADDR_NONE
    }
  };
  client->keys = keys;

  const uint16_t fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt(fd, IPPROTO_TCP, TCP_FASTOPEN, &(bool){true}, sizeof(bool));
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(bool){true}, sizeof(bool));
  setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &(bool){true}, sizeof(bool));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &(uint8_t){FIX_KEEPALIVE_IDLE}, sizeof(uint8_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &(uint8_t){FIX_KEEPALIVE_INTVL}, sizeof(uint8_t));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &(uint8_t){FIX_KEEPALIVE_CNT}, sizeof(uint8_t));
  client->ssl = init_ssl_socket(fd, ssl_ctx);

  dup2(fd, FIX_FILENO);
  close(fd);
}

inline bool handle_fix_connection(const fix_client_t *restrict client, const uint8_t events, const dns_resolver_t *restrict resolver)
{
  static void *restrict states[] = {&&dns_query, &&dns_response, &&connect, &&ssl_handshake, &&send_logon, &&receive_logon, &&send_limit_query, &&receive_limit_query};

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
  connect(FIX_FILENO, &client->addr, sizeof(client->addr));
  sequence++;
  return false;

ssl_handshake:
  log_msg(STR_LEN_PAIR("Performing SSL handshake"));
  sequence += SSL_connect(client->ssl) == true;
  return false;

send_logon:
  log_msg(STR_LEN_PAIR("Sending logon message"));
  sequence += send_logon(client);
  return false;

receive_logon:
  log_msg(STR_LEN_PAIR("Receiving logon message"));
  sequence += receive_logon(client);
  return false;

send_limit_query:
  log_msg(STR_LEN_PAIR("Sending limit query"));
  sequence += send_limit_query(client);
  return false;

receive_limit_query:
  log_msg(STR_LEN_PAIR("Receiving limit query"));
  return receive_limit_query(client);
}

static bool send_logon(const fix_client_t *restrict client)
{
  //TODO
  //Username (553) API_KEY
  //RawData (96) valid signature made with the API key (base64 encoding of the following:
    //text string constructed by concatenating the values of the following fields in this exact order, separated by the SOH character:
      // MsgType (35)
      // SenderCompId (49)
      // TargetCompId (56)
      // MsgSeqNum (34)
      // SendingTime (52)
  //MessageHandling (25035) Unordered(1)
  //ResponseMode (25036) ONLY_ACKS(2)
}

static bool receive_logon(const fix_client_t *restrict client)
{
  //TODO
}

static bool send_limit_query(const fix_client_t *restrict client)
{
  //TODO
}

static bool receive_limit_query(const fix_client_t *restrict client)
{
  //TODO
}

void free_fix(const fix_client_t *restrict client)
{
  free_ssl_socket(client->ssl);
  close(FIX_FILENO);
}