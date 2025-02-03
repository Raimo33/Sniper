/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:57:09 by craimond          #+#    #+#             */
/*   Updated: 2025/02/03 15:38:28 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/http_parser.h"

HOT static bool request_fits_in_buffer(const http_request_t *restrict request, const uint16_t buffer_size);
HOT static uint16_t parse_status_code(char *restrict buffer, uint16_t *status_code, const uint16_t buffer_size);
HOT static uint16_t parse_headers(char *restrict buffer, header_map_t *headers, const uint16_t buffer_size);
HOT static uint16_t parse_body(char *restrict buffer, char *restrict body, header_map_t *headers, const uint16_t buffer_size);
HOT static uint16_t parse_chunked_body(char *restrict buffer, char *restrict *body, const uint16_t buffer_size);
HOT static uint16_t parse_unified_body(char *restrict buffer, char *restrict *body, const char *restrict content_length, const uint16_t buffer_size);
HOT static uint32_t count_chunked_body_len(const char *restrict buffer, const char *restrict body_end);
HOT static uint8_t count_headers(const char *restrict buffer, const char *restrict headers_end);
HOT static void header_map_insert(header_map_t *restrict map, const char *restrict key, const uint16_t key_len, const char *restrict value, const uint16_t value_len);

static const char methods[][sizeof(uint64_t)] = {
  [GET] = "GET",
  [POST] = "POST",
  [PUT] = "PUT",
  [DELETE] = "DELETE"
};
static const uint8_t methods_len[] = {
  [GET] = STR_LEN("GET"),
  [POST] = STR_LEN("POST"),
  [PUT] = STR_LEN("PUT"),
  [DELETE] = STR_LEN("DELETE")
};

static const char versions[][sizeof(uint64_t)] = {
  [HTTP_1_0] = "HTTP/1.0",
  [HTTP_1_1] = "HTTP/1.1"
};
static const uint8_t versions_len[] = {
  [HTTP_1_0] = STR_LEN("HTTP/1.0"),
  [HTTP_1_1] = STR_LEN("HTTP/1.1")
};

static const char clrf[sizeof(uint16_t)] = "\r\n";
static const char colon_space[sizeof(uint16_t)] = ": ";

uint16_t build_http_request(char *restrict buffer, const uint16_t buffer_size, const http_request_t *restrict request)
{
  const char *buffer_start = buffer;

  if (!request_fits_in_buffer(request, buffer_size))
    panic(STR_AND_LEN("Request does not fit in buffer"));

 *(uint64_t *)buffer = *(const uint64_t *)methods[request->method];
  buffer += methods_len[request->method];
  *buffer++ = ' ';

  memcpy(buffer, request->path, request->path_len);
  buffer += request->path_len;
  *buffer++ = ' ';

  *(uint64_t *)buffer = *(const uint64_t *)versions[request->version];
  buffer += versions_len[request->version];
  *(uint16_t *)buffer = *(const uint16_t *)clrf;
  buffer += 2;

  const header_entry_t *header;
  for (uint8_t i = 0; LIKELY(i < request->n_headers); i++)
  {
    PREFETCHR(&request->headers[i + 1], L0);

    header = &request->headers[i];
    memcpy(buffer, header->key, header->key_len);
    buffer += header->key_len;
    *(uint16_t *)buffer = *(const uint16_t *)colon_space;
    buffer += 2;
    memcpy(buffer, header->value, header->value_len);
    buffer += header->value_len;
    *(uint16_t *)buffer = *(const uint16_t *)clrf;
    buffer += 2;
  }

  *(uint16_t *)buffer = *(const uint16_t *)clrf;
  memcpy(buffer, request->body, request->body_len);
  
  return buffer - buffer_start;
}

static bool request_fits_in_buffer(const http_request_t *restrict request, const uint16_t buffer_size)
{
  uint16_t request_len = 0;

  request_len += methods_len[request->method] + 1;
  request_len += request->path_len + 1;
  request_len += versions_len[request->version] + 2;

  for (uint8_t i = 0; LIKELY(i < request->n_headers); i++)
    request_len += request->headers[i].key_len + 2 + request->headers[i].value_len + 2;
  
  request_len += 2 + request->body_len;

  return request_len < buffer_size;
}

bool is_full_http_response(const char *restrict buffer, const uint16_t buffer_size, const uint16_t response_len)
{
  const char *headers_end = memmem(buffer, buffer_size, STR_AND_LEN("\r\n\r\n"));
  if (LIKELY(headers_end == NULL))
    return false;

  //TODO case insensitiveness
  const char *transfer_encoding = memmem(buffer, buffer_size, STR_AND_LEN("Transfer-Encoding:"));
  if (UNLIKELY(transfer_encoding))
    return !!memmem(buffer, buffer_size, STR_AND_LEN("0\r\n\r\n"));

  const char *content_length = memmem(buffer, buffer_size, STR_AND_LEN("Content-Length:"));
  if (LIKELY(content_length))
  {
    const uint16_t headers_len = headers_end - buffer + STR_LEN("\r\n\r\n");
    const uint16_t body_len = atoi(content_length + STR_LEN("Content-Length:"));
    const uint16_t available_space = buffer_size - headers_len;
    fast_assert(body_len < available_space, STR_AND_LEN("Body size exceeds available space"));
    return ((headers_len + body_len) == response_len);
  }

  panic(STR_AND_LEN("No content length or transfer encoding"));
  UNREACHABLE;
}

uint16_t parse_http_response(char *restrict buffer, http_response_t *response, const uint16_t buffer_size)
{
  uint16_t bytes_parsed = 0;
  bytes_parsed += parse_status_code(buffer, &response->status_code, buffer_size);
  bytes_parsed += parse_headers(buffer, &response->headers, buffer_size);
  bytes_parsed += parse_body(buffer, response->body, &response->headers, buffer_size); 
  return bytes_parsed;
}

static uint16_t parse_status_code(char *restrict buffer, uint16_t *status_code, const uint16_t buffer_size)
{
  const char *line_start = buffer;
  char *line_end = memmem(buffer, buffer_size, STR_AND_LEN("\r\n"));
  fast_assert(line_end, STR_AND_LEN("Malformed response: missing clrf"));
  line_end += STR_LEN("\r\n");

  buffer = strchr(buffer, ' ');
  fast_assert(buffer, STR_AND_LEN("No status code found"));
  *status_code = atoi(buffer);

  return line_end - line_start;
}

static uint16_t parse_headers(char *restrict buffer, header_map_t *headers, const uint16_t buffer_size)
{
  const char *headers_start = buffer;
  char *headers_end = memmem(buffer, buffer_size, STR_AND_LEN("\r\n\r\n"));
  fast_assert(headers_end, STR_AND_LEN("Malformed response: missing clrf"));

  headers->n_entries = count_headers(buffer, headers_end);
  headers->entries = (header_entry_t *)calloc(headers->n_entries * HEADER_MAP_DILUTION_FACTOR, sizeof(header_entry_t));

  while (LIKELY(buffer < headers_end))
  {
    const char *key = buffer;
    buffer = strchr(buffer, ':');
    fast_assert(buffer, STR_AND_LEN("Malformed header: missing colon"));
    const uint16_t key_len = buffer - key;

    buffer += 1 + (buffer[1] == ' ');
    const char *value = buffer;
    buffer = memmem(buffer, headers_end - buffer, STR_AND_LEN("\r\n"));
    const uint16_t value_len = buffer - value;

    header_map_insert(headers, key, key_len, value, value_len);

    buffer += STR_LEN("\r\n");
  }
  headers_end += STR_LEN("\r\n\r\n");

  return headers_end - headers_start;
}

static uint16_t parse_body(char *restrict buffer, char *restrict body, header_map_t *restrict headers, const uint16_t buffer_size)
{
  const header_entry_t *transfer_encoding = header_map_get(headers, STR_AND_LEN("transfer-encoding"));
  if (transfer_encoding && memcmp(transfer_encoding->value, STR_AND_LEN("chunked")) == 0)
    return parse_chunked_body(buffer, &body, buffer_size);

  const header_entry_t *content_length = header_map_get(headers, STR_AND_LEN("content-length"));
  if (content_length)
    return parse_unified_body(buffer, &body, content_length->value, buffer_size);

  panic(STR_AND_LEN("No content length or transfer encoding"));
  UNREACHABLE;
}

static uint16_t parse_chunked_body(char *restrict buffer, char *restrict *body, const uint16_t buffer_size)
{
  const char *body_start = buffer;
  const char *body_end = memmem(buffer, buffer_size, STR_AND_LEN("0\r\n\r\n"));
  uint16_t body_used = 0;

  const uint32_t final_body_len = count_chunked_body_len(buffer, body_end);
  *body = (char *)malloc(final_body_len);

  while (LIKELY(buffer < body_end))
  {
    const uint16_t chunk_len = atoi(buffer);
    buffer = memmem(buffer, body_end - buffer, STR_AND_LEN("\r\n"));
    fast_assert(buffer, STR_AND_LEN("Malformed chunk: missing clrf"));
    buffer += STR_LEN("\r\n");
    memcpy(*body + body_used, buffer, chunk_len);
    *body += chunk_len;
    body_used += chunk_len;
  }
  body_end += STR_LEN("0\r\n\r\n");

  return body_end - body_start;
}

static uint16_t parse_unified_body(char *restrict buffer, char *restrict *body, const char *restrict content_length, UNUSED const uint16_t buffer_size)
{
  const uint16_t body_len = atoi(content_length);
  *body = (char *)malloc(body_len);
  memcpy(*body, buffer, body_len);
  return body_len;
}

//TODO simd easy
static uint32_t count_chunked_body_len(const char *restrict buffer, const char *restrict body_end)
{
  uint32_t body_len = 0;
  while (LIKELY(buffer < body_end))
  {
    body_len += atoi(buffer);
    buffer = memmem(buffer, body_end - buffer, STR_AND_LEN("\r\n"));
    buffer += STR_LEN("\r\n");
  }
  return body_len;
}

//TODO simd easy
static uint8_t count_headers(const char *restrict buffer, const char *restrict headers_end)
{
  uint8_t n_headers = 0;
  while (LIKELY(buffer < headers_end))
  {
    buffer = memmem(buffer, headers_end - buffer, STR_AND_LEN("\r\n"));
    buffer += STR_LEN("\r\n");
    n_headers++;
  }
  return n_headers;
}

static void header_map_insert(header_map_t *restrict map, const char *restrict key, const uint16_t key_len, const char *restrict value, const uint16_t value_len)
{
  char *lower_key = strndup(key, key_len);
  char *copied_value = strndup(value, value_len);
  strtolower(lower_key, key_len);

  const uint16_t map_size = map->n_entries * HEADER_MAP_DILUTION_FACTOR;
  const uint16_t original_index = (uint16_t)murmurhash3((uint8_t *)lower_key, key_len, 42) % map_size;
  uint16_t index = original_index;

  for (uint8_t i = 1; UNLIKELY(map->entries[index].key != NULL); i++)
    index = (original_index + i * i) % map_size;

  map->entries[index] = (header_entry_t){lower_key, key_len, copied_value, value_len};
  map->n_entries++;
}

const header_entry_t *header_map_get(const header_map_t *restrict map, const char *restrict key, const uint16_t key_len)
{
  const uint16_t map_size = map->n_entries * HEADER_MAP_DILUTION_FACTOR;
  const uint16_t original_index = (uint16_t)murmurhash3((uint8_t *)key, key_len, 42) % map_size;
  uint16_t index = original_index;

  uint8_t i = 1;
  while (UNLIKELY(map->entries[index].key != NULL))
  {
    if (LIKELY(memcmp(map->entries[index].key, key, key_len) == 0))
      return &map->entries[index];
    index = (original_index + i * i) % map_size;
    i++;
  }

  return NULL;
}

void free_http_response(http_response_t *response)
{
  for (uint8_t i = 0; LIKELY(i < response->headers.n_entries); i++)
  {
    free(response->headers.entries[i].key);
    free(response->headers.entries[i].value);
  }
  free(response->headers.entries);
  free(response->body);
}