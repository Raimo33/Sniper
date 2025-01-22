/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:57:09 by craimond          #+#    #+#             */
/*   Updated: 2025/01/22 21:57:03 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/http_parser.h"

static const str_len_pair_t methods[] = {
  [HTTP_GET]    {STR_LEN_PAIR("GET")},
  [HTTP_POST]   {STR_LEN_PAIR("POST")},
  [HTTP_PUT]    {STR_LEN_PAIR("PUT")},
  [HTTP_DELETE] {STR_LEN_PAIR("DELETE")}
};
static const str_len_pair_t versions[] = {
  [HTTP_1_0] {STR_LEN_PAIR("HTTP/1.0")},
  [HTTP_1_1] {STR_LEN_PAIR("HTTP/1.1")}
};

void build_http_request(const http_request_t *restrict req, char *restrict buf)
{
  memcpy(buf, methods[req->method].str, methods[req->method].len);
  buf += methods[req->method].len;

  *buf++ = ' ';
  
  memcpy(buf, req->path, req->path_len);
  buf += req->path_len;

  *buf++ = ' ';

  memcpy(buf, versions[req->version].str, versions[req->version].len);
  buf += versions[req->version].len;

  static const uint16_t colon_space = (uint16_t)':' << sizeof(char) | ' ';
  static const uint16_t crlf = (uint16_t)'\r' << sizeof(char) | '\n';

  #pragma ivdep
  for (uint8_t i = 0; LIKELY(i < req->headers_count); i++)
  {
    PREFETCHR(&req->headers[i + 1], L0);

    memcpy(buf, req->headers[i].key, req->headers[i].key_len);
    buf += req->headers[i].key_len;
    
    *(uint16_t *)buf = colon_space;
    buf += 2;

    memcpy(buf, req->headers[i].value, req->headers[i].value_len);
    buf += req->headers[i].value_len;
    
    *(uint16_t *)buf = crlf;
    buf += 2;
  }

  *(uint16_t *)buf = crlf;
  buf += 2;

  memcpy(buf, req->body, req->body_len);
  buf += req->body_len;
}

//TODO SIMD
/*
// Required Edge Cases
// Basic Protocol
// Partial/incomplete reads
// Missing CRLF terminators
// Missing headers
// Invalid status codes
// Malformed request lines
// Empty lines before headers
// Security
// Buffer overflow attempts
// Header injection
// Oversized headers/content
// NULL bytes in headers
// Invalid UTF-8 sequences
// Content Handling
// Chunked encoding
// Compressed content (gzip, deflate)
// Multipart/form-data
// Different charsets
*/
void parse_http_response(char *restrict buf, const uint16_t len, http_response_t *restrict res)
{
  char *line = strtok_r(buf, "\r\n", &buf);
  assert(line, STR_LEN_PAIR("Malformed HTTP response: missing status line"));

  line = strchr(line, ' ');
  assert(line, STR_LEN_PAIR("Malformed HTTP response: missing status code"));
  line += 1;

  const uint16_t status_code = (line[0] - '0') * 100 + (line[1] - '0') * 10 + (line[2] - '0');
  assert(status_code >= 100 && status_code <= 599, STR_LEN_PAIR("Malformed HTTP response: invalid status code"));

  header_t *headers = res->headers;
  uint8_t i = 0;
  line = strtok_r(NULL, "\r\n", &buf);
  assert(line, STR_LEN_PAIR("Malformed HTTP response: missing CRLF terminator"));

  while (LIKELY(line[0]))
  {
    headers[i].key = strtok_r(line, ": ", &line);
    headers[i].value = strtok_r(line, ": ", &line);
    line = strtok_r(NULL, "\r\n", &buf);

    assert(headers[i].key && headers[i].value, STR_LEN_PAIR("Malformed HTTP response: missing header"));
    assert(line, STR_LEN_PAIR("Malformed HTTP response: missing CRLF terminator"));

    headers[i].key_len = headers[i].value - headers[i].key - 2;
    headers[i].value_len = line - headers[i].value - 2;
    i++;
  }

  line = strtok_r(NULL, "\r\n", &buf);
  assert(line, STR_LEN_PAIR("Malformed HTTP response: missing CRLF terminator"));

  res->body = strtok_r(NULL, "\r\n", &buf);
  res->headers = headers;
  res->headers_count = i;
  res->status_code = status_code;
  res->body = line;
  res->body_len = len - (res->body - buf);
}
