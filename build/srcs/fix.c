/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 21:02:36 by craimond          #+#    #+#             */
/*   Updated: 2025/01/15 19:02:34 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/fix.h"

static void send_fix_logon(const fix_client_t *fix);
static void receive_fix_logon(const fix_client_t *fix);

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
      if (events & EPOLLOUT)
        send_fix_logon(fix);
      else
        receive_fix_logon(fix);
      sequence++;
      break;
    default:
      break;
  }

  return (sequence >= 3);
}

void handle_fix_event(const fix_client_t *fix)
{
  //TODO
}

static void send_fix_logon(const fix_client_t *fix)
{
  //TODO
}

static void receive_fix_logon(const fix_client_t *fix)
{
  //TODO
}

void free_fix(const fix_client_t *fix)
{
  free_ssl_socket(&fix->ssl_sock);
  close(FIX_FILENO);
}