/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:01:43 by craimond          #+#    #+#             */
/*   Updated: 2025/01/14 14:46:58 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/keys.h"

static void generate_ws_key(WC_RNG *const rng, byte *const key);

void init_keys(keys_t *const keys, WC_RNG *const rng)
{
  const byte *const priv_key = getenv("PRIV_KEY");
  const byte *const api_key  = getenv("API_KEY");

  assert(priv_key && api_key, "Missing environment variables");

  memcpy(keys->api_key, api_key, API_KEY_SIZE);
  wc_ed25519_import_private_only(priv_key, ED25519_PRIV_KEY_SIZE, keys->priv_key);
  generate_ws_key(keys->ws_key, rng);
}

static void generate_ws_key(byte *const key, WC_RNG *const rng)
{
  byte random_data[16];
  wc_RNG_GenerateBlock(rng, random_data, sizeof(random_data));

  word32 encoded_size = WS_KEY_SIZE;
  Base64_Encode(random_data, sizeof(random_data), key, &encoded_size);
}

void free_keys(keys_t *const keys)
{
  wc_ed25519_free(keys->priv_key);
}