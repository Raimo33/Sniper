/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:15:15 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 18:19:37 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <stdint.h>
# include <signal.h>
# include <sys/signalfd.h>

# include "extensions.h"
# include "logger.h"
# include "errors.h"

COLD uint16_t init_signals(void);
COLD void handle_signal(const uint8_t fd, const uint32_t events, void *data);
COLD void free_signals(uint8_t fd);

#endif