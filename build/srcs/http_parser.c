/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:57:09 by craimond          #+#    #+#             */
/*   Updated: 2025/01/18 10:17:08 by craimond         ###   ########.fr       */
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

//TODO buf e req devono essere aligned correttamente (16)
void build_http_request(const http_request_t *req, char *buf)
{
  memcpy(buf, methods[req->method].str, methods[req->method].len);
  buf += methods[req->method].len;

  *buf++ = ' ';
  
  memcpy(buf, req->path, req->path_len);
  buf += req->path_len;

  *buf++ = ' ';

  memcpy(buf, versions[req->version].str, versions[req->version].len);
  buf += versions[req->version].len;

  #pragma ivdep
  for (uint8_t i = 0; LIKELY(i < req->headers_count); i++)
  {
    PREFETCHR(&req->headers[i + 1], L0);

    memcpy(buf, req->headers[i].key, req->headers[i].key_len);
    buf += req->headers[i].key_len;
    *buf++ = ':';
    *buf++ = ' ';
    memcpy(buf, req->headers[i].value, req->headers[i].value_len);
    buf += req->headers[i].value_len;
    *buf++ = '\r';
    *buf++ = '\n';
  }

  *buf++ = '\r';
  *buf++ = '\n';

  if (req->body_len)
    memcpy(buf, req->body, req->body_len);
}

void parse_http_response(const char *buf, http_request_t *req)
{
  //TODO
}

inline uint16_t compute_request_len(const http_request_t *req)
{
  uint16_t len = methods[req->method].len + 1 + req->path_len + versions[req->version].len + 2;

  PREFETCHR(&req->body_len, L0);
  for (uint8_t i = 0; LIKELY(i < req->headers_count); i++)
  {
    PREFETCHR(&req->headers[i + 1].key_len, L0);
    PREFETCHR(&req->headers[i + 1].value_len, L0);
    len += req->headers[i].key_len + 2 + req->headers[i].value_len + 2;
  }
  len += 2;

  len += req->body_len;

  return len;
}