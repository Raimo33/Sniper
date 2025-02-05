/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:05:27 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 12:21:59 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include <stdint.h>

# include "extensions.h"
# include "logger.h"
# include "errno.h"
# include "stdlib.h"
# include "sys/wait.h"

HOT ALWAYS_INLINE extern inline void fast_assert(const bool condition, const char *restrict msg);
COLD extern inline void panic(const char *restrict msg);
HOT void wait_child_process(const pid_t pid);

#endif