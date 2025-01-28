/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:57:09 by craimond          #+#    #+#             */
/*   Updated: 2025/01/28 21:33:08 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/http_parser.h"

static void HOT header_map_insert(header_map_t *restrict map, const char *restrict key, const uint16_t key_len, const char *restrict value, const uint16_t value_len);

uint16_t parse_http_response(char *restrict buffer, http_response_t *response, const uint16_t buffer_size)
{
  const char *headers_end = find_headers_end(buffer, buffer_size);
  if (UNLIKELY(!headers_end))
    return 0;

  if (UNLIKELY(response->body_len > 0))
    goto parse_body;

  parse_status_code(buffer, headers_end, response);
  parse_headers(buffer, headers_end, response);

  if (LIKELY(response->body_len == 0))
    return //TODO returns the number of bytes parsed. ONLY WHEN FULL REQUEST IS PARSED

  response->body = headers_end + 4;
  
parse_body:
  //TODO returns the number of bytes parsed. ONLY WHEN FULL REQUEST IS PARSED
}

char *find_headers_end(char *restrict buffer, uint16_t buffer_size)
{
  const char *headers_end = memmem(buffer, buffer_size, "\r\n\r\n", 4);

}

static void header_map_insert(header_map_t *restrict map, const char *restrict key, const uint16_t key_len, const char *restrict value, const uint16_t value_len)
{
  uint16_t index = (uint16_t)murmurhash3(key, strlen(key), 42) % HEADER_MAP_SIZE;
  uint16_t original_index = index;

  uint8_t i = 1;
  while (UNLIKELY(map->entries[index].key))
  {
    index = (original_index + i * i) % HEADER_MAP_SIZE;
    i++;
  }

  map->entries[index] = (header_entry_t){
    .key = key,
    .value = value,
    .key_len = key_len,
    .value_len = value_len
  };
  map->entries_count++;
}

header_entry_t *header_map_get(header_map_t *restrict map, const char *restrict key, const uint16_t key_len)
{
  uint16_t original_index = (uint16_t)murmurhash3(key, key_len, 42) % HEADER_MAP_SIZE;
  uint16_t index = original_index;

  uint8_t i = 1;
  while (UNLIKELY(map->entries[index].key))
  {
    if (LIKELY(map->entries[index].key_len == key_len && memcmp(map->entries[index].key, key, key_len) == 0))
      return &map->entries[index];
    index = (original_index + i * i) % HEADER_MAP_SIZE;
    i++;
  }

  return NULL;
}