/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:15:15 by craimond          #+#    #+#             */
/*   Updated: 2025/01/30 20:04:50 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <stdint.h>
# include <signal.h>
# include <sys/signalfd.h>

# include "extensions.h"

# define SIG_FILENO 3

void COLD init_signals(void);
void COLD handle_signal(const uint8_t events);
void COLD free_signals(void);

#endif