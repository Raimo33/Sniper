/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 21:02:36 by craimond          #+#    #+#             */
/*   Updated: 2025/01/25 17:38:09 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/fix.h"

static void COLD send_logon(const fix_client_t *restrict client);
static void COLD receive_logon(const fix_client_t *restrict client);
static void COLD send_limit_query(const fix_client_t *restrict client);
static void COLD receive_limit_query(const fix_client_t *restrict client);
static void HOT format_price(const fixed_point_t price, char *buffer); //TODO 5. Zero-Cost String Formatting, Precomputed FIX template (e.g., "44=XXXX|")

//TODO pool di connessioni
void init_fix(fix_client_t *restrict client, const keys_t *restrict keys, const WOLFSSL_CTX *restrict ssl_ctx)
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
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(bool){1}, sizeof(bool));
  client->ssl = init_ssl_socket(fd, ssl_ctx);

  dup2(fd, FIX_FILENO);
  close(fd);
}

inline bool handle_fix_connection(const fix_client_t *restrict client, const uint8_t events, const dns_resolver_t *restrict resolver)
{
  static void *restrict states[] = {&&resolve, &&connect, &&ssl_handshake, &&send_logon, &&receive_logon, &&send_limit_query, &&receive_limit_query};
  static uint8_t sequence = 0;

  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic(STR_LEN_PAIR("FIX connection error"));

  goto *states[sequence];

resolve:
  log(STR_LEN_PAIR("Resolving FIX endpoint: " FIX_HOST));
  sequence += resolve_domain(resolver, STR_LEN_PAIR(FIX_HOST), &client->addr, FIX_FILENO);
  return false;

connect:
  log(STR_LEN_PAIR("Connecting to FIX endpoint: " FIX_HOST));
  connect(FIX_FILENO, &client->addr, sizeof(client->addr));
  sequence++;
  return false;

ssl_handshake:
  log(STR_LEN_PAIR("Performing SSL handshake"));
  sequence += wolfSSL_connect(client->ssl) == SSL_SUCCESS;
  return false;

send_logon:
  log(STR_LEN_PAIR("Sending logon message"));
  sequence += send_logon(client)
  return false;

receive_logon:
  log(STR_LEN_PAIR("Receiving logon message"));
  sequence += receive_logon(client)
  return false;

send_limit_query:
  log(STR_LEN_PAIR("Sending limit query"));
  sequence += send_limit_query(client)
  return false;

receive_limit_query:
  log(STR_LEN_PAIR("Receiving limit query"));
  return receive_limit_query(client);
}

static void send_logon(const fix_client_t *restrict client)
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

static void receive_logon(const fix_client_t *restrict client)
{
  //TODO
}

static void send_limit_query(const fix_client_t *restrict client)
{
  //TODO
}

static void receive_limit_query(const fix_client_t *restrict client)
{
  //TODO
}

void free_fix(const fix_client_t *restrict client)
{
  free_ssl_socket(client->ssl);
  close(FIX_FILENO);
}