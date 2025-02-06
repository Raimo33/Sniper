/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decompression.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:15:24 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 18:19:37 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DECOMPRESSION_H
# define DECOMPRESSION_H

# include <zlib.h>
# include <stdint.h>
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>

# include "extensions.h"

# define PIPE_BUF_SIZE _PC_PIPE_BUF

HOT void gzip_decompress_to_file(uint8_t *input, const uint32_t input_len, uint8_t fd);

#endif