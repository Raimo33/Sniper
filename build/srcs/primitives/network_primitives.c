/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_primitives.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:31:19 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 11:21:26 by craimond         ###   ########.fr       */
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

inline int32_t connect_p(int32_t sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int32_t ret = connect(sockfd, addr, addrlen);
  if (ret == -1)
  {
    printf("DEB CONNECT ERROR\n");
    panic(strerror(errno));
  }
  return ret;
}