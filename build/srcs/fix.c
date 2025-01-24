/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 21:02:36 by craimond          #+#    #+#             */
/*   Updated: 2025/01/24 19:08:52 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/fix.h"

static void COLD send_logon(const fix_client_t *restrict fix);
static void COLD receive_logon(const fix_client_t *restrict fix);
static void COLD send_limit_query(const fix_client_t *restrict fix);
static void COLD receive_limit_query(const fix_client_t *restrict fix);
static void HOT format_price(const fixed_point_t price, char *buffer); //TODO 5. Zero-Cost String Formatting, Precomputed FIX template (e.g., "44=XXXX|")

//TODO pool di connessioni
void init_fix(fix_client_t *restrict fix, const keys_t *restrict keys, const WOLFSSL_CTX *restrict ssl_ctx)
{
  fix->addr = (struct sockaddr_in){
    .sin_family = AF_INET,
    .sin_port = htons(FIX_PORT),
    .sin_addr = {
      .s_addr = inet_addr(FIX_HOST) //TODO getaddrinfo e dns resolve
    }
  };
  fix->keys = keys;

  const uint16_t fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(bool){1}, sizeof(bool));
  fix->ssl = init_ssl_socket(fd, ssl_ctx);

  dup2(fd, FIX_FILENO);
  close(fd);
}

inline bool handle_fix_connection(const fix_client_t *restrict fix, const char fd_state)
{
  static void *restrict states[] = { &&connect, &&ssl_handshake, &&send_logon, &&receive_logon, &&send_limit_query, &&receive_limit_query };
  static uint8_t sequence;

  if (UNLIKELY(fd_state == 'e'))
    panic(STR_LEN_PAIR("FIX connection error"));

  goto *states[sequence];

connect:
  connect(FIX_FILENO, &fix->addr, sizeof(fix->addr));
  sequence++;
  return false;

ssl_handshake:
  sequence += wolfSSL_connect(fix->ssl) == SSL_SUCCESS;
  return false;

send_logon:
  sequence += send_logon(fix)
  return false;

receive_logon:
  sequence += receive_logon(fix)
  return false;

send_limit_query:
  sequence += send_limit_query(fix)
  return false;

receive_limit_query:
  sequence += receive_limit_query(fix)
  return true;
}

static void send_logon(const fix_client_t *restrict fix)
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

static void receive_logon(const fix_client_t *restrict fix)
{
  //TODO
}

static void send_limit_query(const fix_client_t *restrict fix)
{
  //TODO
}

static void receive_limit_query(const fix_client_t *restrict fix)
{
  //TODO
}

void free_fix(const fix_client_t *restrict fix)
{
  free_ssl_socket(fix->ssl);
  close(FIX_FILENO);
}