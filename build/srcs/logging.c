/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:09:22 by craimond          #+#    #+#             */
/*   Updated: 2025/01/08 20:07:12 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/logging.h"
#include "headers/config.h"

static const t_log_level g_levels[] = LOG_LEVELS;

uint8_t map_log_level(const char *level)
{
  const uint8_t n_levels = sizeof(g_levels) / sizeof(t_log_level);

  for (uint8_t i = 0; i < n_levels; i++)
    if (!strcmp(level, g_levels[i].tag))
      return g_levels[i].level;
  return 0;
}

void log(const uint8_t level, const char *msg)
{
  const uint8_t n_levels = sizeof(g_levels) / sizeof(t_log_level);

  //TODO implementare log
}

