/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_primitives.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:00:24 by craimond          #+#    #+#             */
/*   Updated: 2025/02/07 09:54:24 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "primitives/file_primitives.h"

# include <stdio.h> //TODO remove

inline int32_t dup_p(int32_t oldfd)
{
  const int32_t newfd = dup(oldfd);
  if (newfd == -1)
  {
    printf("DEB DUP ERROR\n");
    panic(strerror(errno));
  }
  return newfd;
}

inline int32_t fcntl_p(int32_t fd, int32_t cmd, int32_t arg)
{
  const int32_t ret = fcntl(fd, cmd, arg);
  if (ret == -1)
  {
    printf("DEB FCNTL ERROR\n");
    panic(strerror(errno));
  }
  return ret;
}

inline int32_t socket_p(int32_t domain, int32_t type, int32_t protocol)
{
  const int32_t fd = socket(domain, type, protocol);
  if (fd == -1)
  {
    printf("DEB SOCKET ERROR\n");
    panic(strerror(errno));
  }
  return fd;
}

inline int32_t setsockopt_p(int32_t fd, int32_t level, int32_t optname, const void *optval, socklen_t optlen)
{
  if (setsockopt(fd, level, optname, optval, optlen) == -1)
  {
    printf("DEB SETSOCKOPT ERROR\n");
    panic(strerror(errno));
  }
  return 0;
}

inline int32_t epoll_create1_p(int32_t flags)
{
  const int32_t fd = epoll_create1(flags);
  if (fd == -1)
  {
    printf("DEB EPOLL_CREATE1 ERROR\n");
    panic(strerror(errno));
  }
  return fd;
}

inline int32_t epoll_ctl_p(int32_t epfd, int32_t op, int32_t fd, struct epoll_event *event)
{
  if (epoll_ctl(epfd, op, fd, event) == -1)
  {
    printf("DEB EPOLL_CTL ERROR\n");
    panic(strerror(errno));
  }
  return 0;
}

inline int32_t epoll_wait_p(int32_t epfd, struct epoll_event *events, int32_t maxevents, int32_t timeout)
{
  const int32_t ret = epoll_wait(epfd, events, maxevents, timeout);
  if (ret == -1)
  {
    printf("DEB EPOLL_WAIT ERROR\n");
    panic(strerror(errno));
  }
  return ret;
}

inline int32_t writev_p(int32_t fd, const struct iovec *iov, int32_t iovcnt)
{
  const int32_t ret = writev(fd, iov, iovcnt);
  if (ret == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
  {
    printf("DEB WRITEV ERROR\n");
    panic(strerror(errno));
  }
  return ret;
}

inline int32_t read_p(int32_t fd, void *buf, size_t count)
{
  const int32_t ret = read(fd, buf, count);
  if (ret == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
  {
    printf("DEB READ ERROR\n");
    panic(strerror(errno));
  }
  return ret;
}

inline FILE *fdopen_p(const int32_t fd, const char *mode)
{
  FILE *restrict ret = fdopen(fd, mode);
  if (ret == NULL)
  {
    printf("DEB FDOPEN ERROR\n");
    panic(strerror(errno));
  }
  return ret;
}