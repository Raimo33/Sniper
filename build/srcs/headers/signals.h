/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:15:15 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 13:17:46 by craimond         ###   ########.fr       */
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

# define SIG_FILENO 3

COLD void init_signals(void);
COLD void handle_signal(const uint32_t events);
COLD void free_signals(void);

#endif