/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:15:10 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 16:26:08 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/signals.h"

uint16_t init_signals(void)
{
  sigset_t mask;

  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGTERM);
  sigprocmask(SIG_BLOCK, &mask, NULL);

  return signalfd(-1, &mask, SFD_NONBLOCK);
}

void handle_signal(const uint16_t fd, UNUSED const uint32_t events, UNUSED void *data)
{
  struct signalfd_siginfo info;
  read(fd, &info, sizeof(info));

  switch (info.ssi_signo)
  {
    case SIGINT:
      FALLTHROUGH;
    case SIGTERM:
      panic("Received termination signal");
      UNREACHABLE;
  }
  UNREACHABLE;
}

void free_signals(const uint16_t fd)
{
  close(fd);
}