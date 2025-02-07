/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:07:01 by craimond          #+#    #+#             */
/*   Updated: 2025/02/07 18:06:51 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"

# include <stdio.h> //TODO remove

inline void fast_assert(const bool condition, const char *restrict msg)
{
  if (UNLIKELY(!condition))
    panic(msg);
}

inline void panic(const char *restrict msg)
{
  log_msg(msg, strlen(msg));
  fflush(stderr); //TODO remove
  fflush(stdout); //TODO remove
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