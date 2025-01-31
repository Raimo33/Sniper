/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decompression.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:48:01 by craimond          #+#    #+#             */
/*   Updated: 2025/01/31 18:03:42 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/decompression.h"

 //TODO capire
void decompress_to_pipe(const uint8_t *input, size_t input_len, int *read_pipe)
{
  GzipDecompressor ctx = {0};

  pipe(ctx.pipefd);
  inflateInit2(&ctx.z, 16 + MAX_WBITS);

  ctx.z.next_in = (Bytef *)input;
  ctx.z.avail_in = input_len;
  bool decompression_done = false;

  size_t have;
  while (!decompression_done)
  {
    ctx.z.next_out = ctx.out_buf;
    ctx.z.avail_out = PIPE_BUF_SIZE;

    decompression_done = (inflate(&ctx.z, Z_FINISH) == Z_STREAM_END);

    have = PIPE_BUF_SIZE - ctx.z.avail_out;
    write(ctx.pipefd[1], ctx.out_buf, have);
  }

  inflateEnd(&ctx.z);
  close(ctx.pipefd[1]);
  *read_pipe = ctx.pipefd[0];
}
