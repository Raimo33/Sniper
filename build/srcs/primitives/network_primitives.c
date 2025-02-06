/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_primitives.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:31:19 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 10:32:49 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "primitives/network_primitives.h"

# include <stdio.h> //TODO remove

inline int32_t getaddrinfo_a_p(int32_t mode, struct gaicb *list[], int32_t ent, struct sigevent *sig)
{
  int32_t ret = getaddrinfo_a(mode, list, ent, sig);
  if (ret != 0)
  {
    printf("DEB GETADDRINFO_A ERROR\n");
    panic(gai_strerror(ret));
  }
  return ret;
}