/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_primitives.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:02:51 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 11:01:28 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_PRIMITIVES_H
# define FILE_PRIMITIVES_H

# include <stdint.h>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <unistd.h>
# include <fcntl.h>

# include "extensions.h"
# include "errors.h"

COLD extern inline int32_t dup_p(int32_t oldfd);
COLD extern inline int32_t fcntl_p(int32_t fd, int32_t cmd, int32_t arg);
COLD extern inline int32_t socket_p(int32_t domain, int32_t type, int32_t protocol);
COLD extern inline int32_t setsockopt_p(int32_t fd, int32_t level, int32_t optname, const void *optval, socklen_t optlen);
COLD extern inline int32_t epoll_create1_p(int32_t flags);
COLD extern inline int32_t epoll_ctl_p(int32_t epfd, int32_t op, int32_t fd, struct epoll_event *event);
HOT extern inline int32_t epoll_wait_p(int32_t epfd, struct epoll_event *events, int32_t maxevents, int32_t timeout);
HOT extern inline int32_t writev_p(int32_t fd, const struct iovec *iov, int32_t iovcnt);
HOT extern inline int32_t read_p(int32_t fd, void *buf, size_t count);

#endif