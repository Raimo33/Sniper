/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:57:09 by craimond          #+#    #+#             */
/*   Updated: 2025/01/19 10:35:33 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/http_parser.h"

static const str_len_pair_t methods[] = {
  {STR_LEN_PAIR("GET")},
  {STR_LEN_PAIR("POST")},
  {STR_LEN_PAIR("PUT")},
  {STR_LEN_PAIR("DELETE")}
};
static const str_len_pair_t versions[] = {
  {STR_LEN_PAIR("HTTP/1.0")},
  {STR_LEN_PAIR("HTTP/1.1")}
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

  if (req->body_len)
    memcpy(buf, req->body, req->body_len);
}

void parse_http_response(const char *restrict buf, http_response_t *restrict res)
{
  //TODO strtok, spostare semplicemente i puntatori, SIMD per cercare i separatori in parallelo
}