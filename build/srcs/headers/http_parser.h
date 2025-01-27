/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parser.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:43:46 by craimond          #+#    #+#             */
/*   Updated: 2025/01/27 14:54:53 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_PARSER_H
# define HTTP_PARSER_H

# include <stdint.h>
# include <string.h>

# include "extensions.h"
# include "logger.h"

# define HTTP_GET 0
# define HTTP_POST 1
# define HTTP_PUT 2
# define HTTP_DELETE 3

# define HTTP_1_0 0
# define HTTP_1_1 1

# define MAX_HEADERS 8
# define HEADER_MAP_SIZE MAX_HEADERS * 4

typedef struct
{
  const char *str;
  const uint8_t len;
} str_len_pair_t;

typedef struct
{
  char *key;
  uint8_t key_len;

  char *value;
  uint8_t value_len;
} header_entry_t;

typedef struct
{
  header_entry_t entries[HEADER_MAP_SIZE] ALIGNED(16);
  uint8_t entries_count;
} header_map_t;

typedef struct
{
  const char *path;
  const header_entry_t headers[MAX_HEADERS] ALIGNED(16);
  const char *body;

  const uint16_t body_len;
  const uint8_t path_len;

  const uint8_t headers_count;
  const uint8_t method;
  const uint8_t version;
} http_request_t;

typedef struct
{
  header_map_t headers;
  char *body;

  uint16_t body_len;
  uint16_t status_code;
} http_response_t;

void HOT build_http_request(const http_request_t *restrict req, char *restrict buf);
void HOT parse_http_response(char *restrict buf, const uint16_t len, http_response_t *restrict res);
header_entry_t HOT *header_map_get(header_map_t *restrict map, const char *restrict key, const uint16_t key_len);

#endif