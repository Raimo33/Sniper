/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 21:12:39 by craimond          #+#    #+#             */
/*   Updated: 2025/02/04 20:53:11 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string_utils.h"

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

const char *get_timestamp_utc_str(void)
{
  static time_t last_time;
  static char last_timestamp[UTC_TIMESTAMP_SIZE];

  const time_t now = time_p(NULL);
  
  if (UNLIKELY(now == last_time))
    return last_timestamp;

  struct tm now_utc;
  gmtime_r_p(&now, &now_utc);
  strftime(last_timestamp, UTC_TIMESTAMP_SIZE, "%Y%m%d-%H:%M:%S", &now_utc);
  last_time = now;

  return last_timestamp;
}

uint8_t ultoa(uint64_t num, char *buffer)
{
  if (num == 0)
  {
    buffer[0] = '0';
    return 1;
  }

  const uint8_t digits = 1 +
    (num >= 10UL) +
    (num >= 100UL) +
    (num >= 1000UL) +
    (num >= 10000UL) +
    (num >= 100000UL) +
    (num >= 1000000UL) +
    (num >= 10000000UL) +
    (num >= 100000000UL) +
    (num >= 1000000000UL) +
    (num >= 10000000000UL) +
    (num >= 100000000000UL) +
    (num >= 1000000000000UL) +
    (num >= 10000000000000UL) +
    (num >= 100000000000000UL) +
    (num >= 1000000000000000UL) +
    (num >= 10000000000000000UL) +
    (num >= 100000000000000000UL) +
    (num >= 1000000000000000000UL);

  static const uint64_t power10[] = {
    1UL,
    10UL,
    100UL,
    1000UL,
    10000UL,
    100000UL,
    1000000UL,
    10000000UL,
    100000000UL,
    1000000000UL,
    10000000000UL,
    100000000000UL,
    1000000000000UL,
    10000000000000UL,
    100000000000000UL,
    1000000000000000UL,
    10000000000000000UL,
    100000000000000000UL,
    1000000000000000000UL,
  };

  uint64_t power = power10[digits - 1];
  char *p = buffer;
  uint64_t quotient;
  for (int i = 0; i < digits; i++)
  {
    quotient = num / power;
    *p++ = (char)('0' + quotient);
    num -= quotient * power;
    power /= 10;
  }

  return digits;
}

static inline uint64_t tolower8(uint64_t octets)
{
  const uint64_t all_bytes = 0x0101010101010101;
  const uint64_t heptets = octets & (0x7F * all_bytes);
  const uint64_t is_gt_Z = heptets + (0x7F - 'Z') * all_bytes;
  const uint64_t is_ge_A = heptets + (0x80 - 'A') * all_bytes;
  const uint64_t is_ascii = ~octets & (0x80 * all_bytes);
  const uint64_t is_upper = is_ascii & (is_ge_A ^ is_gt_Z);
  return (octets | (is_upper >> 2));
}