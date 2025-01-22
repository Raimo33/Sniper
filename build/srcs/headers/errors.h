/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:05:27 by craimond          #+#    #+#             */
/*   Updated: 2025/01/22 20:56:08 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include <stdint.h>

# include "extensions.h"
# include "logger.h"

extern void *cleanup_label;

inline void HOT ALWAYS_INLINE assert(const bool condition, const char *restrict msg, const uint8_t len);
void COLD panic(const char *restrict msg, const uint8_t len);

#endif