/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:07:01 by craimond          #+#    #+#             */
/*   Updated: 2025/01/25 21:37:49 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"

inline void assert(const bool condition, const char *restrict msg, const uint8_t len)
{
  if (UNLIKELY(!condition))
    panic(msg, len);
}

void panic(const char *restrict msg, const uint8_t len)
{
  log(msg, len);
  goto *cleanup_label;
}