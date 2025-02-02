/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 21:10:47 by craimond          #+#    #+#             */
/*   Updated: 2025/02/02 19:58:46 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STR_UTILS_H
# define STR_UTILS_H

# include <stdint.h>

# include "extensions.h"

# define STR_LEN_PAIR(s) s, sizeof(s) - 1
# define STR_LEN(s) sizeof(s) - 1

typedef struct
{
  const char *str;
  const uint16_t len;
} str_len_pair_t;

HOT void strtolower(char *str, const uint16_t len);

#endif