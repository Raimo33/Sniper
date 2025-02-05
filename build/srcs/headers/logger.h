/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:08:11 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 16:41:03 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_H
# define LOGGER_H

# include <unistd.h>
# include <string.h>
# include <stdint.h>
# include <stdbool.h>
# include <fcntl.h>
# include <sys/uio.h>
# include <sys/epoll.h>

# include "extensions.h"
# include "errors.h"
# include "string_utils.h"

# define LOG_RING_SIZE 4096

typedef struct
{
  char data[LOG_RING_SIZE] ALIGNED(16);
  uint16_t head;
  uint16_t tail;
} t_log_ring;

COLD uint16_t init_logger(void);
HOT void log_msg(const char *restrict msg, const uint8_t msg_len);
HOT void handle_logs(const uint16_t fd, const uint32_t events, void *data);
COLD void free_logger(uint16_t fd);

#endif