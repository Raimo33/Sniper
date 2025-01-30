/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:15:10 by craimond          #+#    #+#             */
/*   Updated: 2025/01/30 20:04:39 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/signals.h"

void init_signals(void)
{
  sigset_t mask;

  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGTERM);
  sigprocmask(SIG_BLOCK, &mask, NULL);

  const uint16_t fd = signalfd(-1, &mask, SFD_NONBLOCK);
  dup2(fd, SIG_FILENO);
  close(fd);
}

void handle_signal(const uint8_t events)
{
  struct signalfd_siginfo info;
  read(SIG_FILENO, &info, sizeof(info));

  switch (info.ssi_signo)
  {
    case SIGINT: FALLTHROUGH;
    case SIGTERM:
      panic(STR_LEN_PAIR("Received termination signal"));
    default:
      UNREACHABLE;
  }
  UNREACHABLE;
}

void free_signals(void)
{
  close(SIG_FILENO);
}