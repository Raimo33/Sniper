/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixed_point.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:10:57 by craimond          #+#    #+#             */
/*   Updated: 2025/01/30 20:26:03 by craimond         ###   ########.fr       */
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

inline fixed_point_t fast_add(const fixed_point_t a, const fixed_point_t b)
{
  return (fixed_point_t){.value = a.value + b.value, .precision = a.precision};
}

inline fixed_point_t fast_sub(const fixed_point_t a, const fixed_point_t b)
{
  return (fixed_point_t){.value = a.value - b.value, .precision = a.precision};
}

inline fixed_point_t fast_mul(const fixed_point_t a, const fixed_point_t b)
{
  return (fixed_point_t){.value = a.value * b.value, .precision = a.precision + b.precision};
}

inline fixed_point_t fast_div(const fixed_point_t a, const fixed_point_t b)
{
  return (fixed_point_t){.value = (a.value * powers_of_ten[a.precision]) / b.value, .precision = a.precision};
}
