/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decompression.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:15:24 by craimond          #+#    #+#             */
/*   Updated: 2025/01/31 18:04:20 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DECOMPRESSION_H
# define DECOMPRESSION_H

# include <zlib.h>
# include <stdint.h>
# include <stdbool.h>
# include <unistd.h>

# include "extensions.h"

# define PIPE_BUF_SIZE _PC_PIPE_BUF

typedef struct
{
  int pipefd[2];
  z_stream z;
  uint8_t out_buf[PIPE_BUF_SIZE] ALIGNED(16);
  size_t bytes_processed;
} GzipDecompressor;

void HOT decompress_gzip(const uint8_t *input, size_t input_len, const int32_t read_pipe);

#endif