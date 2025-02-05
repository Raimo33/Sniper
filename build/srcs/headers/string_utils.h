/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 21:10:47 by craimond          #+#    #+#             */
/*   Updated: 2025/02/04 20:53:01 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_UTILS_H
# define STRING_UTILS_H

# include <stdint.h>
# include <stdbool.h>
# include <time.h>

# include "extensions.h"

# define STR_AND_LEN(s) s, sizeof(s) - 1
# define STR_LEN(s) sizeof(s) - 1
# define ARR_LEN(a) sizeof(a) / sizeof(a[0])
# define UTC_TIMESTAMP_SIZE STR_LEN("19700101-00:00:00")

HOT void strtolower(char *str, const uint16_t len);
HOT const char *get_timestamp_utc_str(void);
HOT extern inline uint8_t ultoa(uint64_t num, char *buffer);

#endif