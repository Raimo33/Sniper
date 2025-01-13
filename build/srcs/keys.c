/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:01:43 by craimond          #+#    #+#             */
/*   Updated: 2025/01/13 09:59:27 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/keys.h"

static void generate_keys(const WC_RNG *rng, keys_t *const keys);
static void export_keys(const keys_t *const keys);
static void import_keys(keys_t *const keys);
static void prompt_api_key(const char *const pub_key_str, char *const api_key_str);

void init_keys(ssl_data_t *const ssl_data, keys_t *const keys)
{
  const bool keys_exist = (access(PRIVATE_KEY_PATH, F_OK) == -1 || access(API_KEY_PATH, F_OK) == -1);

  if (!keys_exist)
  {
    generate_keys(ssl_data->rng, keys);
    export_keys(keys);
  }
  else
    import_keys(keys);
}

//https://www.wolfssl.com/documentation/manuals/wolfssl/group__ED25519.html
static void generate_keys(const WC_RNG *rng, keys_t *const keys)
{
  wc_ed25519_init(keys->priv_key);
  wc_ed25519_make_key(rng, ED25519_PRIV_KEY_SIZE, keys->priv_key);

  byte pub_key_str[ED25519_PUB_KEY_SIZE];
  wc_ed25519_make_public(keys->priv_key, pub_key_str, ED25519_PUB_KEY_SIZE);

  prompt_api_key(pub_key_str, keys->api_key);
}

static void export_keys(const keys_t *const keys)
{
  //TODO export private, api, and publc keys to kernel
}

static void import_keys(keys_t *const keys)
{
  //TODO import private, api, and public keys from kernel
}

static void prompt_api_key(const char *const pub_key_str, char *const api_key_str)
{
  const char msg1[] = "API key missing!\ned25519 public key: ";
  const char msg2[] = "\nEnter your API key: ";

  const struct iovec iov[3] = {
    {.iov_base = msg1, .iov_len = sizeof(msg1) - 1},
    {.iov_base = pub_key_str, .iov_len = ED25519_PUB_KEY_SIZE},
    {.iov_base = msg2, .iov_len = sizeof(msg2) - 1}
  };
  
  writev(STDOUT_FILENO, iov, sizeof(iov) / sizeof(iov[0]));
  const uint8_t bytes_read = read(STDIN_FILENO, api_key_str, API_KEY_SIZE + 1);
  api_key_str[bytes_read - 1] = '\0';
}

void free_keys(keys_t *const keys)
{
  wc_ed25519_free(keys->priv_key);
  wc_ed25519_free(keys->pub_key);
}