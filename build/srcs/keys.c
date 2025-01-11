/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:01:43 by craimond          #+#    #+#             */
/*   Updated: 2025/01/11 21:52:30 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/keys.h"

void init_keys(ssl_data_t *const ssl_data, keys_t *const keys)
{
  const bool keys_exist = !access(PRIVATE_KEY_PATH, F_OK) && !access(API_KEY_PATH, F_OK);
  const uint16_t key_fd = open(PRIVATE_KEY_PATH, O_RDONLY | O_CREAT | O_EXCL | O_NONBLOCK);
  const uint16_t api_fd = open(API_KEY_PATH, O_RDONLY | O_CREAT | O_EXCL | O_NONBLOCK);

  if (keys_exist)
  {
    //TODO read asynchronously and fill keys (private and api)
    //TODO generate public key from private key and fill keys
  }
  else
  {
    //generate private and public key
    //put private key in file
    //ask user for api key
  }
  //check del formato delle keys? (base64, hex, etc) o lo fa binance?  
}