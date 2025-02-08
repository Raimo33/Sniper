/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 18:59:34 by craimond          #+#    #+#             */
/*   Updated: 2025/02/08 13:16:21 by craimond         ###   ########.fr       */
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

# include "primitives/crypto_primitives.h"
# include "extensions.h"
# include "logger.h"

# define API_KEY_SIZE 64
# define WS_KEY_SIZE 16
# define ED25519_KEYLEN 32
# define ED25519_SIGSIZE 64
# define SHA1_DIGEST_SIZE 20
# define WS_MAGIC_GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
# define BASE64_ENCODED_SIZE(x) (((x) + 2) / 3 * 4)
# define BASE64_DECODED_SIZE(x) (((x) / 4) * 3)

typedef struct
{
  EVP_PKEY *priv_key;
  uint8_t api_key[API_KEY_SIZE];
} keys_t;

COLD void init_keys(keys_t *restrict keys);
COLD void generate_ws_key(char *restrict buffer, const uint8_t buffer_size);
COLD bool verify_ws_key(const uint8_t *restrict key, const uint8_t key_len, const uint8_t *restrict accept, const uint16_t accept_len);
HOT void sign_ed25519(EVP_PKEY *key, const char *data, const uint16_t data_len, char *restrict buffer);
COLD void free_keys(keys_t *restrict keys);

#endif