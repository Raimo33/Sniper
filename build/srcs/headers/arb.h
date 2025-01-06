/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arb.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 17:05:09 by craimond          #+#    #+#             */
/*   Updated: 2025/01/06 01:32:28 by craimond         ###   ########.fr       */
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

# include "utils.h"

# define REST_ENDPOINT "api3.binance.com:443"
# define WS_ENDPOINT "data-stream.binance.vision:9443"
# define FIX_ENDPOINT "fix-oe.binance.com:9000"
# define PAIRS = { "usdcust", "fdusdusdt", "usdtdai", "usdpusdt", "fdusdusdc", "eureuri"}
# define LOGGING_INTERVAL 60 * 1000
# define MAX_MARKET_ORDER_DELAY 50
# define MAX_LIMIT_ORDER_DELAY 1000

#endif