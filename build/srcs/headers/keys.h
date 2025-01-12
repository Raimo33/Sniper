/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 18:59:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/12 16:15:46 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYS_H
# define KEYS_H

# include <stdint.h>
# include <wolfssl/wolfcrypt/ed25519.h>

# define PRIVATE_KEY_PATH "/keys/ed25519.key"
# define API_KEY_PATH "/keys/api.key"
# define ED25519_PUB_KEY_SIZE 32
# define ED25519_PRIV_KEY_SIZE 32
# define API_KEY_SIZE 64

typedef struct
{
  const ed25519_key priv_key;
  const ed25519_key pub_key;
  const char[API_KEY_SIZE] api_key;
} keys_t;

void init_keys(keys_t *const keys);
void free_keys(keys_t *const keys);

#endif