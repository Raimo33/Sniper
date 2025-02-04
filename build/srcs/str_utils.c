/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 21:12:39 by craimond          #+#    #+#             */
/*   Updated: 2025/02/04 20:53:11 by craimond         ###   ########.fr       */
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

const char *get_timestamp_utc_str(void)
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

uint8_t ultoa(uint64_t num, char *buffer)
{
  if (num == 0)
  {
    buffer[0] = '0';
    return 1;
  }

  static const uint64_t power10[] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
  };

  int low = 0, high = 18, digits = 0;
  while (LIKELY(low <= high))
  {
    int mid = (low + high) / 2;
    if (num >= power10[mid])
    {
      digits = mid + 1;
      low = mid + 1;
    }
    else
      high = mid - 1;
  }

  uint64_t power = power10[digits - 1];
  char *p = buffer;
  for (uint8_t i = 0; LIKELY(i < digits); ++i)
  {
    uint64_t digit = num / power;
    *p++ = (char)('0' + digit);
    num -= digit * power;
    power /= 10;
  }

  return digits;
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