/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixed_point.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:01:52 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 21:56:19 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_POINT_H
# define FIXED_POINT_H

# include <stdint.h>
# include <stdbool.h>
# include <math.h>
# include <limits.h>

# include "extensions.h"

# define FIXED_NEG_INF ((fixed_point_t){.value = INT_MIN, .precision = 0})
# define FIXED_INF ((fixed_point_t){.value = INT_MAX, .precision = 0})
# define FIXED_ZERO ((fixed_point_t){.value = 0, .precision = 0})
# define FIXED_ONE ((fixed_point_t){.value = 1, .precision = 0})

typedef struct
{
  int32_t value;
  uint8_t precision;
} fixed_point_t;

HOT extern inline fixed_point_t ALWAYS_INLINE fast_add(const fixed_point_t a, const fixed_point_t b);
HOT extern inline fixed_point_t ALWAYS_INLINE fast_sub(const fixed_point_t a, const fixed_point_t b);
HOT extern inline fixed_point_t ALWAYS_INLINE fast_mul(const fixed_point_t a, const fixed_point_t b);
HOT extern inline fixed_point_t ALWAYS_INLINE fast_div(const fixed_point_t a, const fixed_point_t b);
HOT extern inline fixed_point_t ALWAYS_INLINE fast_log2(const fixed_point_t a);

#endif