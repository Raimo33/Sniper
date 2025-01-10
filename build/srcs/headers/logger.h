/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:08:11 by craimond          #+#    #+#             */
/*   Updated: 2025/01/10 18:55:46 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_H
# define LOGGER_H

# include <unistd.h>
# include <stdint.h>
# include <fcntl.h>
# include <sys/uio.h>

# define LOG_LEVEL_DEBUG 0U
# define LOG_LEVEL_INFO  1U
# define LOG_LEVEL_WARN  2U
# define LOG_LEVEL_ERROR 3U

# define LOG_RING_SIZE 4096U
# define LOG_FD        7U

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
} t_log_ring;

uint8_t map_log_level(const char *const level);
void    init_logger(void);
void    log(const uint8_t level, const char *const msg, const uint8_t msg_len);
void    flush_logs(void);

#endif