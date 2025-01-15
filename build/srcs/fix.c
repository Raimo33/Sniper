/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 21:02:36 by craimond          #+#    #+#             */
/*   Updated: 2025/01/15 19:34:00 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/fix.h"

static void send_logon(const fix_client_t *fix);
static void receive_logon(const fix_client_t *fix);
static void send_limit_query(const fix_client_t *fix);
static void receive_limit_query(const fix_client_t *fix);

//TODO pool di connessioni
void init_fix(fix_client_t *fix, const keys_t *keys)
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
  init_ssl_socket(fd, &fix->ssl_sock);

  dup2(fd, FIX_FILENO);
  close(fd);
}

bool handle_fix_connection_event(const fix_client_t *fix, const uint32_t events)
{
  static uint8_t sequence;

  switch (sequence)
  {
    case 0:
      connect(FIX_FILENO, (struct sockaddr *)&fix->addr, sizeof(fix->addr));
      sequence++;
      break;
    case 1:
      if (wolfSSL_connect(fix->ssl_sock.ssl) == SSL_SUCCESS)
        sequence++;
      break;
    case 2:
      (events & EPOLLOUT) ? send_logon(fix) : receive_logon(fix);
      sequence++;
      break;
    case 3:
      (events & EPOLLOUT) ? send_limit_query(fix) : receive_limit_query(fix);
      sequence++;
      break;
    default:
      break;
  }

  return (sequence >= 4);
}

void handle_fix_event(const fix_client_t *fix)
{
  //TODO
}

static void send_logon(const fix_client_t *fix)
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

static void receive_logon(const fix_client_t *fix)
{
  //TODO
}

static void send_limit_query(const fix_client_t *fix)
{
  //TODO
}

static void receive_limit_query(const fix_client_t *fix)
{
  //TODO
}

void free_fix(const fix_client_t *fix)
{
  free_ssl_socket(&fix->ssl_sock);
  close(FIX_FILENO);
}