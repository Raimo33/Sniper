/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:51 by craimond          #+#    #+#             */
/*   Updated: 2025/01/21 17:52:38 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIX_H
# define FIX_H

# include <stdint.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <netdb.h>

# include "extensions.h"
# include "ssl.h"
# include "http_parser.h"
# include "shared_buffers.h"

# define FIX_HOST "fix-oe.binance.com"
# define FIX_PORT 9000
# define FIX_FILENO 4

typedef struct
{
  const struct sockaddr_in addr;
  SSL *ssl;
  const keys_t *keys;
} fix_client_t;

void COLD init_fix(fix_client_t *restrict fix, const keys_t *restrict keys, const WOLFSSL_CTX *restrict ssl_ctx);
inline bool HOT handle_fix_connection(const fix_client_t *restrict fix, const char fd_state);
void COLD free_fix(const fix_client_t *restrict fix);

#endif
