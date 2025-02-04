/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 21:12:39 by craimond          #+#    #+#             */
/*   Updated: 2025/02/04 16:45:38 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "str_utils.h"

HOT static inline uint64_t tolower8(uint64_t octets);

void strtolower(char *str, uint16_t len)
{
  uint64_t *chunk = (uint64_t *)str;

  for (; len >= 8; len -= 8, chunk++)
    *chunk = tolower8(*chunk);

  char *remaining = (char *)chunk;
  for (; len; len--, remaining++)
    *remaining += (*remaining >= 'A' && *remaining <= 'Z') * 32;
}

inline const char *get_timestamp_utc_str(void)
{
  static time_t last_time;
  static char last_timestamp[UTC_TIMESTAMP_SIZE];

  const time_t now = time(NULL);
  
  if (UNLIKELY(now == last_time))
    return last_timestamp;

  struct tm now_utc;
  gmtime_r(&now, &now_utc);
  strftime(last_timestamp, UTC_TIMESTAMP_SIZE, "%Y%m%d-%H:%M:%S", &now_utc);
  last_time = now;

  return last_timestamp;
}

static inline uint64_t tolower8(uint64_t octets)
{
  uint64_t all_bytes = 0x0101010101010101;
  uint64_t heptets = octets & (0x7F * all_bytes);
  uint64_t is_gt_Z = heptets + (0x7F - 'Z') * all_bytes;
  uint64_t is_ge_A = heptets + (0x80 - 'A') * all_bytes;
  uint64_t is_ascii = ~octets & (0x80 * all_bytes);
  uint64_t is_upper = is_ascii & (is_ge_A ^ is_gt_Z);
  return (octets | (is_upper >> 2));
}