/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_primitives.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:00:52 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 10:30:10 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "primitives/sys_primitives.h"

# include <stdio.h> //TODO remove

inline int32_t setrlimit_p(int32_t resource, const struct rlimit *rlim)
{
  if (setrlimit(resource, rlim) == -1)
  {
    printf("DEB SETRLIMIT ERROR\n"); 
    panic(strerror(errno));
  }
  return 0;
}