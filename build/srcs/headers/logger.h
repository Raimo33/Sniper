/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:08:11 by craimond          #+#    #+#             */
/*   Updated: 2025/01/28 21:19:49 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_H
# define LOGGER_H

# include <unistd.h>
# include <stdint.h>
# include <stdbool.h>
# include <fcntl.h>
# include <sys/uio.h>
# include <sys/epoll.h>

# include "extensions.h"
# include "str_utils.h"

# define LOG_RING_SIZE 4096
# define LOG_FILENO 7

typedef struct
{
  char data[LOG_RING_SIZE] ALIGNED(16);
  uint16_t head;
  uint16_t tail;
} t_log_ring;

void COLD init_logger(void);
void HOT log_msg(const char *restrict msg, const uint8_t msg_len);
inline void HOT handle_logs(const uint8_t events);
void HOT flush_logs(void);
void COLD free_logger(void);

#endif