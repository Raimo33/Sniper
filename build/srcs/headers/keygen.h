/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keygen.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 18:59:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/11 19:02:13 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYGEN_H
# define KEYGEN_H

# include <stdint.h>
# include <wolfssl/wolfcrypt/ed25519.h>

# define KEYS_DIR "/keys"
# define ED25519_KEY_SIZE 32
# define API_KEY_SIZE 64

typedef struct
{
  const char api_key[API_KEY_SIZE];
  const char private_key[ED25519_KEY_SIZE]; 
} keys_t;

void generate_keys(keys_t *const keys);

#endif