/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:05:27 by craimond          #+#    #+#             */
/*   Updated: 2025/01/30 20:26:27 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include <stdint.h>

# include "extensions.h"
# include "logger.h"
# include "errno.h"
# include "stdlib.h"

extern inline void HOT ALWAYS_INLINE fast_assert(const bool condition, const char *restrict msg, const uint8_t len);
void COLD panic(const char *restrict msg, const uint8_t len);

#endif