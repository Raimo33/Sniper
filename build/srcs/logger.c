/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:09:22 by craimond          #+#    #+#             */
/*   Updated: 2025/02/07 09:46:35 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logger.h"

HOT static void flush_logs(const uint8_t fd);

static t_log_ring g_log_ring;

uint16_t init_logger(void)
{
  const uint8_t fd = dup_p(STDOUT_FILENO);
  const uint8_t flags = fcntl_p(fd, F_GETFL, 0);
  fcntl_p(fd, F_SETFL, flags | O_NONBLOCK);

  return fd;
}

void log_msg(const char *restrict msg, const uint8_t msg_len)
{
  const uint16_t next_head = (g_log_ring.head + msg_len + 1) % LOG_RING_SIZE;
  
  if (UNLIKELY(next_head == g_log_ring.tail))
    return;

  char *restrict dest = &g_log_ring.data[g_log_ring.head];
  memcpy(dest, msg, msg_len);
  dest[msg_len] = '\n';

  g_log_ring.head = next_head;
}

void handle_logs(const uint8_t fd, const uint32_t events, UNUSED void *data)
{
  if (LIKELY(events & EPOLLOUT))
    flush_logs(fd);
  else
    panic("Error on logger fd");
}

static void flush_logs(const uint8_t fd)
{
  struct iovec iov[2];
  uint8_t iovcnt = 1;

  if (UNLIKELY(g_log_ring.head == g_log_ring.tail))
    return;

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

  const uint16_t written = writev_p(fd, iov, iovcnt);
  if (written > 0)
    g_log_ring.tail = (g_log_ring.tail + written) % LOG_RING_SIZE;
}

void free_logger(uint8_t fd)
{
  flush_logs(fd);
  close(fd);
}

