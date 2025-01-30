/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 21:12:39 by craimond          #+#    #+#             */
/*   Updated: 2025/01/30 19:07:25 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "str_utils.h"

static inline uint64_t tolower8(uint64_t octets);

static void strtolower(char *str, uint16_t len)
{
  uint64_t *chunk = (uint64_t *)str;

  for (; len >= 8; len -= 8, chunk++)
    *chunk = tolower8(*chunk);

  char *remaining = (char *)chunk;
  for (; len; len--, remaining++)
    *remaining += (*remaining >= 'A' && *remaining <= 'Z') * 32;
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