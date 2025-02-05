/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:07:01 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 22:23:30 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"

inline void fast_assert(const bool condition, const char *restrict msg)
{
  if (UNLIKELY(!condition))
    panic(msg);
}

inline void panic(const char *restrict msg)
{
  log_msg(msg, strlen(msg));
  {
    //TODO remove this block, only for debugging
    printf("PANIC: errno: %d, %s\n", errno, strerror(errno));
  }
  exit(EXIT_FAILURE);
}

void wait_child_process(const pid_t pid)
{
  int32_t status;

  waitpid(pid, &status, 0);
  if (WIFEXITED(status))
  {
    const int8_t exit_status = WEXITSTATUS(status);
    if (exit_status != EXIT_SUCCESS)
      exit(exit_status);
  }
  else
    exit(EXIT_FAILURE);
}