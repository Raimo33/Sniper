/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:09:22 by craimond          #+#    #+#             */
/*   Updated: 2025/01/25 15:24:33 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/logger.h"

static t_log_ring g_log_ring;

void init_logger(void)
{
  dup2(STDOUT_FILENO, LOG_FILENO);
  const uint8_t flags = fcntl(LOG_FILENO, F_GETFL, 0);
  fcntl(LOG_FILENO, F_SETFL, flags | O_NONBLOCK);
}

void log(const char *restrict msg, const uint8_t msg_len)
{
  const uint16_t next_head = (g_log_ring.head + msg_len + 1) % LOG_RING_SIZE;
  
  if (UNLIKELY(next_head == g_log_ring.tail))
    return;

  char *restrict dest = &g_log_ring.data[g_log_ring.head];
  memcpy(dest, msg, msg_len);
  dest[msg_len] = '\n';

  g_log_ring.head = next_head;
}

void handle_logs(const uint8_t events)
{
  if (LIKELY(events & EPOLLOUT))
    flush_logs();
  else
    panic(STR_LEN_PAIR("Error on logger fd"));
}

void flush_logs(void)
{
  struct iovec iov[2];
  uint8_t iovcnt = 1;

  if (UNLIKELY(g_log_ring.head == g_log_ring.tail))
    return;

  PREFETCHR(&g_log_ring.data[g_log_ring.tail], L0);

  if (LIKELY(g_log_ring.head > g_log_ring.tail))
  {
    iov[0].iov_base = &g_log_ring.data[g_log_ring.tail];
    iov[0].iov_len = g_log_ring.head - g_log_ring.tail;
  }
  else
  {
    iov[0].iov_base = &g_log_ring.data[g_log_ring.tail];
    iov[0].iov_len = LOG_RING_SIZE - g_log_ring.tail;
    if (g_log_ring.head > 0)
    {
      iov[1].iov_base = g_log_ring.data;
      iov[1].iov_len = g_log_ring.head;
      iovcnt = 2;
    }
  }

  const uint16_t written = writev(LOG_FILENO, iov, iovcnt);
  if (written > 0)
    g_log_ring.tail = (g_log_ring.tail + written) % LOG_RING_SIZE;
  // TODO: Handle EAGAIN/EWOULDBLOCK
}

void free_logger(void)
{
  flush_logs();
  close(LOG_FILENO);
}

