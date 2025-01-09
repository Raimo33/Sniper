/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:08:11 by craimond          #+#    #+#             */
/*   Updated: 2025/01/09 17:30:46 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGING_H
# define LOGGING_H

# include <unistd.h>
# include <stdint.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>

# define LOG_LEVEL_DEBUG 0
# define LOG_LEVEL_INFO  1
# define LOG_LEVEL_WARN  2
# define LOG_LEVEL_ERROR 3

# define STDOUT_FILENO 1

# define LOG_RING_SIZE 4096

typedef struct
{
  const char    *tag;
  const uint8_t level   : 2;
  const uint8_t tag_len : 3;
} t_log_level;

typedef struct
{
  char     data[LOG_RING_SIZE];
  uint16_t head;
  uint16_t tail;
  uint8_t  fd;
} t_log_ring;

uint8_t map_log_level(const char *level);
void    init_logger(void);
void    log(const uint8_t level, const char *msg, const uint8_t msg_len);
void    flush_logs(void);

#endif