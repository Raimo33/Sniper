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

//TODO simd
uint16_t serialize_fix_fields(char *buffer, const uint16_t buffer_size, const fix_field_t *fields, const uint16_t n_fields)
{
  const char *buffer_start = buffer;

  //TODO check della dimensione del buffer (come http serializer), quanto rallenta??  

  //TODO copy all tags of the body and their values

  return buffer - buffer_start;
}

uint16_t finalize_fix_message(char *buffer, const uint16_t buffer_size)
{
  //TODO beginstring
  //TODO bodylength
  //TODO checksum
  return //TODO length of the total message including added tags;
}