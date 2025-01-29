/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:57:09 by craimond          #+#    #+#             */
/*   Updated: 2025/01/29 15:39:03 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/http_parser.h"

static void HOT header_map_insert(header_map_t *restrict map, const char *restrict key, const uint16_t key_len, const char *restrict value, const uint16_t value_len);

bool is_full_http_response(const char *restrict buffer, const uint16_t buffer_size)
{
  if (memmem(buffer, buffer_size, "0\r\n\r\n", 4) != NULL)
    return true;
  //assert se non c'e' risposta entro bufer size. risposta troppo grande che non entra nel buffer
  const char *headers_end = memmem(buffer, buffer_size, "\r\n\r\n", 4);
  if (headers_end == NULL)
    return false;

  const char *content_length = memmem(buffer, buffer_size, STR_LEN_PAIR("Content-Length:")); //TODO case insensitiveness
}

uint16_t parse_http_response(char *restrict buffer, http_response_t *response, const uint16_t buffer_size)
{
  //TODOclean, role based parsing all in one. fast AF!!
  //TODO strlower
  //TODO ritorna parsed_bytes
}

static void header_map_insert(header_map_t *restrict map, const char *restrict key, const uint16_t key_len, const char *restrict value, const uint16_t value_len)
{
  uint16_t index = (uint16_t)murmurhash3(key, strlen(key), 42) % HEADER_MAP_SIZE;
  uint16_t original_index = index;

  for (uint8_t i = 1; map->entries[index].key; i++)
    index = (original_index + i * i) % HEADER_MAP_SIZE;

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
    if (LIKELY(memcmp(map->entries[index].key, key, key_len) == 0))
      return &map->entries[index];
    index = (original_index + i * i) % HEADER_MAP_SIZE;
    i++;
  }

  return NULL;
}