/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:05:27 by craimond          #+#    #+#             */
/*   Updated: 2025/01/17 17:36:58 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include <stdint.h>

# include "logger.h"

extern void *cleanup_label;

/*
TODO
Use __attribute__((pure)) for functions that:

Do not modify any global state.
Have a return value that depends only on their parameters and/or global variables.
May read global variables but do not modify them.
Use __attribute__((const)) for functions that:

Do not read or modify any global state.
Have a return value that depends only on their parameters.
Are more restrictive than pure.

*/

void __attribute__((hot))  assert(const bool condition, const char *msg, const uint8_t len);
void __attribute__((cold)) panic(const char *msg, const uint8_t len);

#endif