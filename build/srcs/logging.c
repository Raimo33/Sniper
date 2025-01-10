/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:09:22 by craimond          #+#    #+#             */
/*   Updated: 2025/01/10 16:58:55 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/logging.h"
#include "headers/config.h"
#include <immintrin.h>

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
  dup2(LOG_FD, STDOUT_FILENO);
  close(STDOUT_FILENO);
  const uint8_t flags = fcntl(LOG_FD, F_GETFL, 0);
  fcntl(LOG_FD, F_SETFL, flags | O_NONBLOCK);
}

void log(const uint8_t level, const char *const msg, const uint8_t msg_len)
{
  if (level > g_config.log_level)
    return;

  uint16_t total_len = g_levels[level].tag_len + 2 +  msg_len;
  uint16_t next_head = (g_log_ring.head + total_len) % LOG_RING_SIZE;
  
  if (next_head == g_log_ring.tail)
    return;

  char *dest = &g_log_ring.data[g_log_ring.head];
  memcpy(dest, g_levels[level].tag, g_levels[level].tag_len);
  dest += g_levels[level].tag_len;
  memcpy(dest, ": ", 2);
  dest += 2;
  memcpy(dest, msg, msg_len);

  g_log_ring.head = next_head;
}

//TODO da daggiungere nell'event loop, da chiamare sempre prima dell'uscita del programma
void flush_logs(void)
{
  uint16_t available;
  uint16_t written;

  while (g_log_ring.tail != g_log_ring.head)
  {
    if (g_log_ring.tail < g_log_ring.head)
      available = g_log_ring.head - g_log_ring.tail;
    else
      available = LOG_RING_SIZE - g_log_ring.tail;

    written = write(LOG_FD, &g_log_ring.data[g_log_ring.tail], available); //TODO error handling EAGAIN EWOULDBLOCK (wrapper? troppo inefficiente?)
    g_log_ring.tail = (g_log_ring.tail + written) % LOG_RING_SIZE;
  }
}

