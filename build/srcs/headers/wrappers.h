/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 21:20:01 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 21:59:55 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRAPPERS_H
# define WRAPPERS_H

# include <stdint.h>
# include <errno.h>
# include <sys/resource.h>
# include <sys/signalfd.h>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <fcntl.h>
# include <signal.h>
# include <openssl/ssl.h>
# include <openssl/err.h>

# include "errors.h"

HOT extern inline void *malloc_p(size_t size);
HOT extern inline void *calloc_p(size_t nmemb, size_t size);

COLD extern inline int32_t setrlimit_p(int32_t resource, const struct rlimit *rlim);
COLD extern inline int32_t dup_p(int32_t oldfd);
COLD extern inline int32_t fcntl_p(int32_t fd, int32_t cmd, int32_t arg);
COLD extern inline int32_t sigemptyset_p(sigset_t *set);
COLD extern inline int32_t sigaddset_p(sigset_t *set, int signum);
COLD extern inline int32_t sigprocmask_p(int how, const sigset_t *set, sigset_t *oldset);
COLD extern inline int32_t signalfd_p(int fd, const sigset_t *mask, int flags);
COLD extern inline int32_t socket_p(int32_t domain, int32_t type, int32_t protocol);
COLD extern inline int32_t setsockopt_p(int32_t fd, int32_t level, int32_t optname, const void *optval, socklen_t optlen);

COLD extern inline int32_t OPENSSL_init_ssl_p(uint32_t opts, const OPENSSL_INIT_SETTINGS *settings);
COLD extern inline SSL_CTX *SSL_CTX_new_p(const SSL_METHOD *method);
COLD extern inline int32_t SSL_CTX_set_min_proto_version_p(SSL_CTX *ctx, int32_t version);
COLD extern inline int32_t SSL_CTX_set_max_proto_version_p(SSL_CTX *ctx, int32_t version);
COLD extern inline SSL *SSL_new_p(SSL_CTX *ctx);
COLD extern inline int32_t SSL_set_fd_p(SSL *ssl, int32_t fd);
COLD extern inline int32_t epoll_create1_p(int32_t flags);
COLD extern inline int32_t epoll_ctl_p(int32_t epfd, int32_t op, int32_t fd, struct epoll_event *event);

#endif