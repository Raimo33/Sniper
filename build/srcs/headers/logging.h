/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:08:11 by craimond          #+#    #+#             */
/*   Updated: 2025/01/08 20:07:31 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGING_H
# define LOGGING_H

# include <unistd.h>
# include <stdint.h>

# define LOG_LEVEL_DEBUG 0
# define LOG_LEVEL_INFO  1
# define LOG_LEVEL_WARN  2
# define LOG_LEVEL_ERROR 3

# define LOG_LEVELS { \
  { LOG_LEVEL_DEBUG, STDOUT_FILENO, "DEBUG" }, \
  { LOG_LEVEL_INFO,  STDOUT_FILENO, "INFO"  }, \
  { LOG_LEVEL_WARN,  STDERR_FILENO, "WARN"  }, \
  { LOG_LEVEL_ERROR, STDERR_FILENO, "ERROR" }  \
}

typedef struct s_log_level
{
  const uint8_t level : 2;
  const uint8_t fd    : 2;
  const char    *tag
} t_log_level;

uint8_t map_log_level(const char *level);

#endif