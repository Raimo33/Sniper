/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:57:09 by craimond          #+#    #+#             */
/*   Updated: 2025/01/15 20:22:14 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/http_parser.h"

//TODO buf e req devono essere aligned correttamente (16)
void build_http_request(const http_request_t *req, char *buf)
{
  switch (req->method)
  {
    case HTTP_GET:
      memcpy(buf, STR_SIZE("GET "));
      break;
    case HTTP_POST:
      memcpy(buf, STR_SIZE("POST "));
      break;
    case HTTP_PUT:
      memcpy(buf, STR_SIZE("PUT "));
      break;
    case HTTP_DELETE:
      memcpy(buf, STR_SIZE("DELETE "));
      break;
    default:
      panic(STR_SIZE("Invalid HTTP method"));
  }
  memcpy(buf, 
}

void parse_http_response(const char *buf, http_request_t *req)
{
  //TODO
}