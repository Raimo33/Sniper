/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crypto_primitives.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 13:14:16 by craimond          #+#    #+#             */
/*   Updated: 2025/02/08 13:15:13 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CRYPTO_PRIMITIVES_H
# define CRYPTO_PRIMITIVES_H

# include <stdint.h>
# include <openssl/err.h>
# include <openssl/rand.h>

# include "extensions.h"
# include "errors.h"

COLD extern inline EVP_PKEY *EVP_PKEY_new_raw_private_key_p(int type, ENGINE *e, const uint8_t *key, size_t keylen);
HOT extern inline int32_t EVP_DigestSignInit_p(EVP_MD_CTX *ctx, EVP_PKEY_CTX **pctx, const EVP_MD *type, ENGINE *e, EVP_PKEY *pkey);
HOT extern inline int32_t EVP_DigestSign_p(EVP_MD_CTX *ctx, uint8_t *sigret, size_t *siglen, const uint8_t *tbs, size_t tbslen);
HOT extern inline int32_t EVP_EncodeBlock_p(uint8_t *t, const uint8_t *f, int n);
HOT extern inline int32_t EVP_DecodeBlock_p(uint8_t *t, const uint8_t *f, int n);
HOT extern inline int32_t RAND_bytes_p(uint8_t *buf, int num);

#endif