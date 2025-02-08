/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simd.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:54:26 by craimond          #+#    #+#             */
/*   Updated: 2025/02/08 14:55:09 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMD_H
# define SIMD_H

# include <immintrin.h>

# define SIMD_STRIDE_AVX512 64
# define SIMD_STRIDE_AVX2 32
# define SIMD_STRIDE_SSE2 16

#endif