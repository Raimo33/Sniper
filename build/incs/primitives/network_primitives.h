/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_primitives.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:34:14 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 11:21:11 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORK_PRIMITIVES_H
# define NETWORK_PRIMITIVES_H

# include <stdint.h>
# include <netdb.h>

# include "extensions.h"
# include "errors.h"

COLD extern inline int32_t getaddrinfo_a_p(int32_t mode, struct gaicb *list[], int32_t ent, struct sigevent *sig);
COLD extern inline int32_t connect_p(int32_t sockfd, const struct sockaddr *addr, socklen_t addrlen);

#endif