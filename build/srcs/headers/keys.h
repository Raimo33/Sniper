/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 18:59:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/27 13:13:11 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYS_H
# define KEYS_H

# include <stdint.h>
# include <openssl/rand.h>
# include <openssl/sha.h>
# include <openssl/evp.h>
# include <openssl/ec.h>
# include <openssl/pem.h>
# include <openssl/bio.h>
# include <openssl/buffer.h>

# include "extensions.h"

# define ED25519_PRIV_KEY_SIZE 32
# define API_KEY_SIZE 64
# define WS_KEY_SIZE 24
# define WS_KEY_GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
# define SHA256_DIGEST_SIZE 32

typedef struct
{
  const ed25519_key priv_key;
  const uint8_t[API_KEY_SIZE] api_key;
} keys_t;

void COLD init_keys(keys_t *restrict keys);
void COLD generate_ws_keys(uint8_t *restrict key);
bool COLD verify_ws_key(const uint8_t *restrict key, const char *restrict accept, const uint16_t len);
uint32_t HOT murmurhash3(const uint8_t *key, const uint16_t len, const uint32_t seed);
void COLD free_keys(keys_t *restrict keys);

#endif