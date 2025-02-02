/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decompression.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:48:01 by craimond          #+#    #+#             */
/*   Updated: 2025/02/02 14:42:02 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/decompression.h"

//https://www.zlib.net/manual.html#Stream

void gzip_decompress_to_file(uint8_t *input, const uint32_t input_len, uint16_t fd)
{
  z_stream strm = {
    .zalloc = Z_NULL,
    .zfree = Z_NULL,
    .opaque = Z_NULL,
    .next_in = input,
    .avail_in = input_len,
  };
  uint8_t out[PIPE_BUF_SIZE];
  int16_t ret;
  
  inflateInit2(&strm, 16 + MAX_WBITS);

  do
  {
    strm.next_out = out;
    strm.avail_out = PIPE_BUF_SIZE;

    ret = inflate(&strm, Z_NO_FLUSH);

    PREFETCHR(strm.next_in, L0);

    write(fd, out, PIPE_BUF_SIZE - strm.avail_out);
  } while (LIKELY(ret != Z_STREAM_END));

  inflateEnd(&strm);
}