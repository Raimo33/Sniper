/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 21:02:36 by craimond          #+#    #+#             */
/*   Updated: 2025/01/19 10:00:22 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/fix.h"

static void send_logon(const fix_client_t *restrict fix);
static void receive_logon(const fix_client_t *restrict fix);
static void send_limit_query(const fix_client_t *restrict fix);
static void receive_limit_query(const fix_client_t *restrict fix);

//TODO pool di connessioni
void init_fix(fix_client_t *restrict fix, const keys_t *restrict keys)
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

//TODO handle_fix_connection
inline bool handle_fix_connection(const fix_client_t *restrict fix, const char fd_state)
{
  static bool connected;

  //TODO state machine con goto

  return connected;
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
  free_ssl_socket(&fix->ssl_sock);
  close(FIX_FILENO);
}