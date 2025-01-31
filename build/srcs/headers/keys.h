/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 18:59:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/31 10:22:32 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYS_H
# define KEYS_H

# include <stdint.h>
# include <stdbool.h>
# include <openssl/rand.h>
# include <openssl/sha.h>
# include <openssl/evp.h>
# include <openssl/ec.h>
# include <openssl/pem.h>
# include <openssl/bio.h>
# include <openssl/buffer.h>

# include "extensions.h"
# include "logger.h"

# define ED25519_PRIV_KEY_SIZE 32
# define API_KEY_SIZE 64
# define WS_KEY_SIZE 24
# define WS_KEY_GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

typedef struct
{
  EVP_PKEY *priv_key;
  uint8_t api_key[API_KEY_SIZE] ALIGNED(16);
} keys_t;

void COLD init_keys(keys_t *restrict keys);
void COLD generate_ws_key(uint8_t *restrict key);
bool COLD verify_ws_key(const uint8_t *restrict key, const uint8_t *restrict accept, const uint16_t len);
uint32_t HOT murmurhash3(const uint8_t *key, const uint16_t len, const uint32_t seed);
void COLD free_keys(keys_t *restrict keys);

#endif