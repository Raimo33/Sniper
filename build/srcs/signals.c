/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:15:10 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 11:01:10 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

uint16_t init_signals(void)
{
  sigset_t mask;

  sigemptyset_p(&mask);
  sigaddset_p(&mask, SIGINT);
  sigaddset_p(&mask, SIGTERM);
  sigprocmask_p(SIG_BLOCK, &mask, NULL);

  return signalfd_p(-1, &mask, SFD_NONBLOCK);
}

void handle_signal(const uint8_t fd, UNUSED const uint32_t events, UNUSED void *data)
{
  struct signalfd_siginfo info;
  read_p(fd, &info, sizeof(info));

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

void free_signals(const uint8_t fd)
{
  close(fd);
}