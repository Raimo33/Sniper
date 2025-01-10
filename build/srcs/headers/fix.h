/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:51 by craimond          #+#    #+#             */
/*   Updated: 2025/01/10 21:33:16 by craimond         ###   ########.fr       */
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
# include <wolfssl/options.h>
# include <wolfssl/ssl.h>

# define FIX_FD  5U
# define FIX_HOST "fix-oe.binance.com"
# define FIX_PORT 9000U

void init_fix(void);
//TODO handler

#endif
