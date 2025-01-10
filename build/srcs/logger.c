/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:09:22 by craimond          #+#    #+#             */
/*   Updated: 2025/01/10 18:55:46 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/logger.h"
#include "headers/config.h"

static t_log_ring        g_log_ring = {0};
static const t_log_level g_levels[] = {
  {"DEBUG", LOG_LEVEL_DEBUG, 5},
  {"INFO",  LOG_LEVEL_INFO,  4},
  {"WARN",  LOG_LEVEL_WARN,  4},
  {"ERROR", LOG_LEVEL_ERROR, 5}
};

uint8_t map_log_level(const char *const level)
{
  const uint8_t n_levels = sizeof(g_levels) / sizeof(g_levels[0]);
  
  for (uint8_t i = 0; i < n_levels; i++)
    if (!strcmp(level, g_levels[i].tag))
      return g_levels[i].level;
  return 0;
}

void init_logger(void)
{
  dup2(STDOUT_FILENO, LOG_FD);
  close(STDOUT_FILENO);
  const uint8_t flags = fcntl(LOG_FD, F_GETFL, 0);
  fcntl(LOG_FD, F_SETFL, flags | O_NONBLOCK);
}

void log(const uint8_t level, const char *const msg, const uint8_t msg_len)
{
  if (level > g_config.log_level)
    return;

  uint16_t total_len = g_levels[level].tag_len + 2 +  msg_len + 1;
  uint16_t next_head = (g_log_ring.head + total_len) % LOG_RING_SIZE;
  
  if (next_head == g_log_ring.tail)
    return;

  char *dest = &g_log_ring.data[g_log_ring.head];
  memcpy(dest, g_levels[level].tag, g_levels[level].tag_len);
  dest += g_levels[level].tag_len;
  memcpy(dest, ": ", 2);
  dest += 2;
  memcpy(dest, msg, msg_len);
  dest[msg_len] = '\n';

  g_log_ring.head = next_head;
}

//TODO da daggiungere nell'event loop, da chiamare sempre prima dell'uscita del programma
void flush_logs(void)
{
  struct iovec iov[2];
  uint8_t iovcnt = 1;

  if (g_log_ring.tail == g_log_ring.head)
    return;

  if (g_log_ring.tail < g_log_ring.head)
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

  const uint16_t written = writev(LOG_FD, iov, iovcnt);
  if (written > 0)
    g_log_ring.tail = (g_log_ring.tail + written) % LOG_RING_SIZE;
  // TODO: Handle EAGAIN/EWOULDBLOCK
}

