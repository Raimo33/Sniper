/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:57:09 by craimond          #+#    #+#             */
/*   Updated: 2025/01/23 16:10:22 by craimond         ###   ########.fr       */
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

static void to_lowercase_simd(char *str, uint16_t len);

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

void parse_http_response(char *restrict buf, const uint16_t len, http_response_t *restrict res)
{
  char *line = strtok_r(buf, "\r\n", &buf);
  assert(line, STR_LEN_PAIR("Malformed HTTP response: missing status line"));

  line = strchr(line, ' ');
  assert(line, STR_LEN_PAIR("Malformed HTTP response: missing status code"));
  line += 1;

  res->status_code = (line[0] - '0') * 100 + (line[1] - '0') * 10 + (line[2] - '0');
  assert(res->status_code >= 100 && res->status_code <= 599, STR_LEN_PAIR("Malformed HTTP response: invalid status code"));

  uint8_t i = 0;
  line = strtok_r(NULL, "\r\n", &buf);
  assert(line, STR_LEN_PAIR("Malformed HTTP response: missing CRLF terminator"));

  while (LIKELY(line[0]))
  {
    PREFETCHR(&res->headers[i + 1], L0);

    res->headers[i].key = strtok_r(line, ": ", &line);
    res->headers[i].value = strtok_r(line, ": ", &line);
    line = strtok_r(NULL, "\r\n", &buf);

    assert(res->headers[i].key && res->headers[i].value, STR_LEN_PAIR("Malformed HTTP response: missing header"));
    assert(line, STR_LEN_PAIR("Malformed HTTP response: missing CRLF terminator"));

    to_lowercase_simd(res->headers[i].key, res->headers[i].key_len);

    if (UNLIKELY(strcmp(res->headers[i].key, "transfer-encoding") == 0))
      panic(STR_LEN_PAIR("Transfer-Encoding not supported"));

    res->headers[i].key_len = res->headers[i].value - res->headers[i].key - 2;
    res->headers[i].value_len = line - res->headers[i].value - 2;
    i++;
  }

  res->headers_count = i;
  res->body = strtok_r(NULL, "\r\n", &buf);
  res->body_len = len - (line - buf) * (res->body != NULL);
}

static void to_lowercase_simd(char *str, uint16_t len)
{
  __m128i upper_A = _mm_set1_epi8('A');
  __m128i upper_Z = _mm_set1_epi8('Z');
  __m128i diff = _mm_set1_epi8('a' - 'A');

  while (LIKELY(len >= 16))
  {
    __m128i chars = _mm_loadu_si128((__m128i *)(str));
    __m128i mask = _mm_and_si128(_mm_cmpgt_epi8(chars, upper_A), _mm_cmplt_epi8(chars, upper_Z));
    __m128i lower_chars = _mm_add_epi8(chars, _mm_and_si128(mask, diff));
    _mm_storeu_si128((__m128i *)(str), lower_chars);
    str += 16;
    len -= 16;
  }

  while (LIKELY(len--))
  {
    if (UNLIKELY(*str >= 'A' && *str <= 'Z'))
      *str += 25;
    str++;
  }
}