/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_primitives.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 09:58:58 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 10:29:14 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "primitives/signal_primitives.h"

# include <stdio.h> //TODO remove

inline int32_t sigemptyset_p(sigset_t *set)
{
  if (sigemptyset(set) == -1)
  {
    printf("DEB SIGEMPTYSET ERROR\n");
    panic(strerror(errno));
  }
  return 0;
}

inline int32_t sigaddset_p(sigset_t *set, int signum)
{
  if (sigaddset(set, signum) == -1)
  {
    printf("DEB SIGADDSET ERROR\n");
    panic(strerror(errno));
  }
  return 0;
}

inline int32_t sigprocmask_p(int how, const sigset_t *set, sigset_t *oldset)
{
  if (sigprocmask(how, set, oldset) == -1)
  {
    printf("DEB SIGPROCMASK ERROR\n");
    panic(strerror(errno));
  }
  return 0;
}

inline int signalfd_p(int fd, const sigset_t *mask, int flags)
{
  const int ret = signalfd(fd, mask, flags);
  if (ret == -1)
  {
    printf("DEB SIGNALFD ERROR\n");
    panic(strerror(errno));
  }
  return ret;
}