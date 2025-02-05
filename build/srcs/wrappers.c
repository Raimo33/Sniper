/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 21:12:58 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 22:20:27 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/wrappers.h"

inline void *malloc_p(size_t size)
{
  void *ptr = malloc(size);
  if (ptr == NULL)
  {
    printf("DEB MALLOC ERROR\n");
    panic(strerror(errno));
  }
  return ptr;
}

inline void *calloc_p(size_t nmemb, size_t size)
{
  void *ptr = calloc(nmemb, size);
  if (ptr == NULL)
  {
    printf("DEB CALLOC ERROR\n");
    panic(strerror(errno));
  }
  return ptr;
}

inline int32_t setrlimit_p(int32_t resource, const struct rlimit *rlim)
{
  if (setrlimit(resource, rlim) == -1)
  {
    printf("DEB SETRLIMIT ERROR\n"); 
    panic(strerror(errno));
  }
  return 0;
}

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

inline int32_t OPENSSL_init_ssl_p(uint32_t opts, const OPENSSL_INIT_SETTINGS *settings)
{
  if (OPENSSL_init_ssl(opts, settings) == 0)
  {
    printf("DEB OPENSSL_INIT_SSL ERROR\n");
    const uint64_t error_code = ERR_get_error();
    char error_msg[256] = {0};
    ERR_error_string_n(error_code, error_msg, sizeof(error_msg));
    panic(error_msg);
  }
  return 0;
}

inline SSL_CTX *SSL_CTX_new_p(const SSL_METHOD *method)
{
  SSL_CTX *ctx = SSL_CTX_new(method);
  if (ctx == NULL)
  {
    printf("DEB SSL_CTX_NEW ERROR\n");
    const uint64_t error_code = ERR_get_error();
    char error_msg[256] = {0};
    ERR_error_string_n(error_code, error_msg, sizeof(error_msg));
    panic(error_msg);
  }
  return ctx;
}

inline int32_t SSL_CTX_set_min_proto_version_p(SSL_CTX *ctx, int32_t version)
{
  if (SSL_CTX_set_min_proto_version(ctx, version) == 0)
  {
    printf("DEB SSL_CTX_SET_MIN_PROTO_VERSION ERROR\n");
    const uint64_t error_code = ERR_get_error();
    char error_msg[256] = {0};
    ERR_error_string_n(error_code, error_msg, sizeof(error_msg));
    panic(error_msg);
  }
  return 0;
}

inline int32_t SSL_CTX_set_max_proto_version_p(SSL_CTX *ctx, int32_t version)
{
  if (SSL_CTX_set_max_proto_version(ctx, version) == 0)
  {
    printf("DEB SSL_CTX_SET_MAX_PROTO_VERSION ERROR\n");
    const uint64_t error_code = ERR_get_error();
    char error_msg[256] = {0};
    ERR_error_string_n(error_code, error_msg, sizeof(error_msg));
    panic(error_msg);
  }
  return 0;
}

inline SSL *SSL_new_p(SSL_CTX *ctx)
{
  SSL *ssl = SSL_new(ctx);
  if (ssl == NULL)
  {
    printf("DEB SSL_NEW ERROR\n");
    const uint64_t error_code = ERR_get_error();
    char error_msg[256] = {0};
    ERR_error_string_n(error_code, error_msg, sizeof(error_msg));
    panic(error_msg);
  }
  return ssl;
}

inline int32_t SSL_set_fd_p(SSL *ssl, int32_t fd)
{
  const int32_t ret = SSL_set_fd(ssl, fd);
  if (ret == 0)
  {
    printf("DEB SSL_SET_FD ERROR\n");
    const uint64_t error_code = ERR_get_error();
    char error_msg[256] = {0};
    ERR_error_string_n(error_code, error_msg, sizeof(error_msg));
    panic(error_msg);
  }
  return ret;
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