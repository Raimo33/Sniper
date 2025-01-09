/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:15:10 by craimond          #+#    #+#             */
/*   Updated: 2025/01/09 18:38:17 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/signals.h"

atomic_bool g_running = true;

static void	signal_handler(int signum);

void  init_signals(void)
{
  struct sigaction sa;
  const uint8_t signals[] = CRITICAL_SIGNALS;
  const uint8_t n_signals = sizeof(signals) / sizeof(signals[0]);

  sa.sa_handler = signal_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  for (uint8_t i = 0; i < n_signals; i++)
    sigaction(signals[i], &sa, NULL);
}

static void	signal_handler(int signum)
{
  if (signum == SIGINT || signum == SIGTERM)
    g_running = false;
}