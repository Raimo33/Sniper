/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@protonmail.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:07:42 by craimond          #+#    #+#             */
/*   Updated: 2024/12/30 00:49:51 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libwebsockets.h>
#include <curl/curl.h>
#include <nghttp2/nghttp2.h>
#include <ngtcp2/ngtcp2.h>
#include <brotli/decode.h>
#include <brotli/encode.h>
#include <zlib.h>
#include <zstd.h>
#include <uv.h>
#include <openssl/ssl.h>
#include <jemalloc/jemalloc.h>
#include <yyjson.h>
