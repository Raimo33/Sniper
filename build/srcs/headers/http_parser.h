/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parser.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:43:46 by craimond          #+#    #+#             */
/*   Updated: 2025/01/19 18:40:12 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_PARSER_H
# define HTTP_PARSER_H

# include <stdint.h>

# include "extensions.h"
# include "logger.h"

# define HTTP_GET 0
# define HTTP_POST 1
# define HTTP_PUT 2
# define HTTP_DELETE 3

# define HTTP_1_0 0
# define HTTP_1_1 1

# define MAX_REQUEST_LEN 4096
# define MAX_RESPONSE_LEN 4096

typedef struct
{
  const char *str;
  const uint8_t len;
} str_len_pair_t;

typedef struct
{
  const char *key;
  const uint16_t key_len;

  const char *value;
  const uint16_t value_len;
} header_t;

typedef struct
{
  const header_t *headers;
  const uint8_t headers_count;

  const char *body;
  const uint16_t body_len;

  const char *path;
  const uint8_t path_len;

  const uint8_t method  : 2;
  const uint8_t version : 1;
} http_request_t;

typedef struct
{
  const header_t *headers;
  const uint8_t headers_count;

  const char *body;
  const uint16_t body_len;

  const uint16_t status_code : 10;
  const uint8_t version : 1;
} http_response_t;

void HOT build_http_request(const http_request_t *restrict req, char *restrict buf, uint16_t *restrict len);
void HOT parse_http_response(const char *restrict buf, http_response_t *restrict res);

#endif