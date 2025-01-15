/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:57:09 by craimond          #+#    #+#             */
/*   Updated: 2025/01/15 21:31:58 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/http_parser.h"

//TODO buf e req devono essere aligned correttamente (16)
void build_http_request(const http_request_t *req, char *buf)
{
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

  char *ptr = buf;

  memcpy(ptr, methods[req->method].str, methods[req->method].len);
  ptr += methods[req->method].len;

  *ptr++ = ' ';
  
  memcpy(ptr, req->path, req->path_len);
  ptr += req->path_len;

  *ptr++ = ' ';

  memcpy(ptr, versions[req->version].str, versions[req->version].len);
  ptr += versions[req->version].len;

  for (uint8_t i = 0; i < req->headers_count; i++)
  {
    memcpy(ptr, req->headers[i].key, req->headers[i].key_len);
    ptr += req->headers[i].key_len;
    *ptr++ = ':';
    *ptr++ = ' ';
    memcpy(ptr, req->headers[i].value, req->headers[i].value_len);
    ptr += req->headers[i].value_len;
    *ptr++ = '\r';
    *ptr++ = '\n';
  }

  *ptr++ = '\r';
  *ptr++ = '\n';

  if (req->body_len)
    memcpy(ptr, req->body, req->body_len);
}

void parse_http_response(const char *buf, http_request_t *req)
{
  //TODO
}