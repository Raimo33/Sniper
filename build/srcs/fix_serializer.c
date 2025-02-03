/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix_serializer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 15:36:24 by craimond          #+#    #+#             */
/*   Updated: 2025/02/02 15:36:27 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/fix_serializer.h"

uint16_t fix_serializer(fix_header_t *restrict header, char *restrict buffer, const uint16_t buffer_size)
{
  const char *buffer_start = buffer;

  //TODO check della dimensione del buffer (come http serializer), quanto rallenta??

  serialize_header(header, buffer);

  //in base al MSG_TYPE eseguira' funzioni diverse. logon ad esempio fara' il signing del payload
  //calcola la checksum, la lunghezza del body e serializza

  return buffer - buffer_start;
}