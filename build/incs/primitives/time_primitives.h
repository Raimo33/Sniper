/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_primitives.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:04:22 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 11:10:26 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIME_PRIMITIVES_H
# define TIME_PRIMITIVES_H

# include <stdint.h>
# include <time.h>

# include "extensions.h"
# include "errors.h"

HOT extern inline time_t time_p(time_t *tloc);
HOT extern inline struct tm *gmtime_r_p(const time_t *timep, struct tm *result);

#endif