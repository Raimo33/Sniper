/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:05:27 by craimond          #+#    #+#             */
/*   Updated: 2025/01/31 21:39:35 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include <stdint.h>

# include "extensions.h"
# include "logger.h"
# include "errno.h"
# include "stdlib.h"

HOT ALWAYS_INLINE extern inline void fast_assert(const bool condition, const char *restrict msg, const uint8_t len);
COLD extern inline void panic(const char *restrict msg, const uint8_t len);

#endif