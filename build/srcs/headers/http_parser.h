/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parser.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:43:46 by craimond          #+#    #+#             */
/*   Updated: 2025/01/31 17:12:21 by craimond         ###   ########.fr       */
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

# define MAX_HEADERS 8
# define HEADER_MAP_SIZE MAX_HEADERS * 4
# define MAX_HEADER_KEY_SIZE 32
# define MAX_HEADER_VALUE_SIZE 64
# define MAX_BODY_SIZE 1024

typedef struct
{
  char key[MAX_HEADER_KEY_SIZE];
  char value[MAX_HEADER_VALUE_SIZE];
  uint8_t key_len;
  uint8_t value_len;
} header_entry_t;

typedef struct
{
  header_entry_t entries[HEADER_MAP_SIZE];
  uint8_t entries_count;
} header_map_t;

typedef struct
{
  header_map_t headers;
  char body[MAX_BODY_SIZE];
  uint16_t body_len;
  uint16_t status_code;
} http_response_t;

bool HOT is_full_http_response(const char *restrict buffer, const uint16_t buffer_size, const uint16_t response_len);
uint16_t HOT parse_http_response(char *restrict buffer, http_response_t *restrict response, const uint16_t buffer_size);
const header_entry_t HOT *header_map_get(const header_map_t *restrict map, const char *restrict key, const uint16_t key_len);

#endif