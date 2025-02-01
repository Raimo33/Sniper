/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parser.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:43:46 by craimond          #+#    #+#             */
/*   Updated: 2025/02/01 22:01:53 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_PARSER_H
# define HTTP_PARSER_H

# include <stdint.h>
# include <string.h>
# include <stdlib.h>

# include "extensions.h"
# include "logger.h"
# include "errors.h"
# include "str_utils.h"
# include "keys.h"

# define HEADER_MAP_DILUTION_FACTOR 3

typedef struct
{
  char *key;
  char *value;
  uint8_t key_len;
  uint8_t value_len;
} header_entry_t;

typedef struct
{
  header_entry_t *entries;
  uint8_t n_entries;
} header_map_t;

typedef struct
{
  header_map_t headers;
  char *body;
  uint16_t body_len;
  uint16_t status_code;
} http_response_t;

HOT bool is_full_http_response(const char *restrict buffer, const uint16_t buffer_size, const uint16_t response_len);
HOT uint16_t parse_http_response(char *restrict buffer, http_response_t *restrict response, const uint16_t buffer_size);
HOT const header_entry_t *header_map_get(const header_map_t *restrict map, const char *restrict key, const uint16_t key_len);
HOT void free_http_response(http_response_t *response);

#endif