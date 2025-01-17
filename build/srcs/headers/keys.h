/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 18:59:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/17 19:55:43 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYS_H
# define KEYS_H

# include <stdint.h>
# include <wolfssl/wolfcrypt/ed25519.h>
# include <wolfssl/wolfcrypt/base64.h>

# include "extensions.h"

# define ED25519_PRIV_KEY_SIZE 32
# define API_KEY_SIZE 64
# define WS_KEY_SIZE 24

typedef struct
{
  const ed25519_key priv_key;
  const byte[API_KEY_SIZE] api_key;
} keys_t;

void COLD init_keys(keys_t *keys, WC_RNG *rng);
void COLD free_keys(keys_t *keys, WC_RNG *rng);

#endif