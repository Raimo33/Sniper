/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parser.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:43:46 by craimond          #+#    #+#             */
/*   Updated: 2025/01/15 20:33:08 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_PARSER_H
# define HTTP_PARSER_H

# include <stdint.h>

# define HTTP_GET 0
# define HTTP_POST 1
# define HTTP_PUT 2
# define HTTP_DELETE 3

# define HTTP_1_0 0
# define HTTP_1_1 1

//TODO assert all these things when parsing.
# define MAX_HEADERS 64
# define MAX_HEADER_KEY_SIZE 64
# define MAX_HEADER_VALUE_SIZE 256
# define MAX_PATH_SIZE 256
# define MAX_BODY_SIZE 4096

# define TOTAL_REQUEST_SIZE ( \
  6 + 1 + MAX_PATH_SIZE + 1 + 8 + 2 + \
  MAX_HEADERS * (MAX_HEADER_KEY_SIZE + 1 + 1 + MAX_HEADER_VALUE_SIZE + 2) + \
  MAX_BODY_SIZE + \
  2 \
)

typedef struct
{
  char key[MAX_HEADER_KEY_SIZE];
  char value[MAX_HEADER_VALUE_SIZE];
} header_t;

typedef struct
{
  header_t headers[MAX_HEADERS];
  char body[MAX_BODY_SIZE];
  char path[MAX_PATH_SIZE];
  uint8_t method  : 2;
  uint8_t version : 1;
} http_request_t;

typedef struct
{
  header_t headers[MAX_HEADERS];
  char body[MAX_BODY_SIZE];
  uint16_t status_code;
  uint8_t version : 1;
} http_response_t;

void build_http_request(const http_request_t *req, char *buf);
void parse_http_response(const char *buf, http_request_t *req);

#endif