/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arb.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 17:05:09 by craimond          #+#    #+#             */
/*   Updated: 2025/01/08 19:42:56 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARB_H
# define ARB_H

# include <arpa/inet.h>
# include <brotli/decode.h>
# include <brotli/encode.h>
# include <fcntl.h>
# include <jemalloc/jemalloc.h>
# include <netinet/in.h>
# include <openssl/err.h>
# include <openssl/ssl.h>
# include <yyjson.h>

# include "config.h"
# include "logging.h"

//TODO epoll, nonblocking sockets, custom JSON parser, jemalloc, nonblocking dns, high-resolution timers, in-memory order books (binary heap), preallocated stack memory, CPU caching, SIMD, memory alignment, async logging (in-memory ring buffer), dont use OpenSSL (heavy), TCP_NODELAY, Set appropriate SO_RCVBUF and SO_SNDBUF,  


#endif