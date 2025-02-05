/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_tweaks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:09:57 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 21:19:26 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/system_tweaks.h"

void set_fd_limit(const uint16_t limit)
{
  const struct rlimit rlim = {
    .rlim_cur = limit,
    .rlim_max = limit
  };

  setrlimit_p(RLIMIT_NOFILE, &rlim);
}