/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixed_point.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:10:57 by craimond          #+#    #+#             */
/*   Updated: 2025/01/25 09:44:43 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/fixed_point.h"

//TODO overflow detection?
//TODO division by zero?
//TODO simd support

static const int32_t powers_of_ten[] ALIGNED(64) = {
  1,
  10,
  100,
  1000,
  10000,
  100000,
  1000000,
  10000000,
  100000000,
  1000000000
};

static inline bool mul_overflow(const int64_t a, const int64_t b, int64_t *result);

inline fixed_point_t add(const fixed_point_t a, const fixed_point_t b)
{
  return (fixed_point_t){.value = a.value + b.value, .precision = a.precision};
}

inline fixed_point_t sub(const fixed_point_t a, const fixed_point_t b)
{
  return (fixed_point_t){.value = a.value - b.value, .precision = a.precision};
}

inline fixed_point_t mul(const fixed_point_t a, const fixed_point_t b)
{
  return (fixed_point_t){a.value * b.value, a.precision + b.precision};
}

inline fixed_point_t div(const fixed_point_t a, const fixed_point_t b)
{
  return (fixed_point_t){a.value * powers_of_ten[a.precision] / b.value, a.precision};
}
