/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 18:59:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/11 21:48:27 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYS_H
# define KEYS_H

# include <stdint.h>
# include <wolfssl/wolfcrypt/ed25519.h>

# define PRIVATE_KEY_PATH "/keys/ed25519.key"
# define API_KEY_PATH "/keys/api.key"
# define ED25519_KEY_SIZE 32
# define API_KEY_SIZE 64

typedef struct
{
  const char api_key[API_KEY_SIZE];
  const char private_key[ED25519_KEY_SIZE];
  const char public_key[ED25519_KEY_SIZE];
} keys_t;

void init_keys(keys_t *const keys);

#endif