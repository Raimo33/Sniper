/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:01:43 by craimond          #+#    #+#             */
/*   Updated: 2025/01/12 13:42:00 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/keys.h"

static void map_files(char *const priv_key_str, char *const api_key_str);
static void map_existing_files(keys_t *const keys);
static void map_new_files(keys_t *const keys);
static void generate_keys(const WC_RNG *rng, ed25519_key *const priv_key, ed25519_key *const pub_key);
static void import_keys(keys_t *const keys, const char *const priv_key_str, const char *const api_key_str);
static void unmap_files(char *const priv_key_file_str, char *const api_key_file_str);

void init_keys(ssl_data_t *const ssl_data, keys_t *const keys)
{
  char priv_key_file_str[ED25519_KEY_SIZE];
  char api_key_file_str[API_KEY_SIZE];
  const bool keys_exist = map_files(priv_key_file_str, api_key_file_str);

  if (!keys_exist)
  {
    generate_keys(ssl_data->rng, keys->priv_key, keys->pub_key);
    //TODO ask user for api key, metterlo in keys->api_key
    export_keys(keys, priv_key_file_str, api_key_file_str);
  }
  else
    import_keys(keys, priv_key_file_str, api_key_file_str);
  unmap_files(priv_key_file_str, api_key_file_str);
}

static void map_files(char *const priv_key_str, char *const api_key_str)
{
  const bool keys_exist = access(PRIVATE_KEY_PATH, F_OK) != -1 && access(API_KEY_PATH, F_OK) != -1; 

  if (keys_exist)
    map_existing_files(keys);
  else
    map_new_files(keys);
  
  return keys_exist;
}

static void map_existing_files(keys_t *const keys)
{
  const uint16_t key_fd = open(PRIVATE_KEY_PATH, O_RDONLY);
  const uint16_t api_fd = open(API_KEY_PATH, O_RDONLY);

  struct stat key_stat;
  struct stat api_stat;
  fstat(key_fd, &key_stat);
  fstat(api_fd, &api_stat);

  //TODO: check if the keys are the right size, or maybe just truncate them
  // assert(key_stat.st_size == ED25519_KEY_SIZE)
  // assert(api_stat.st_size == API_KEY_SIZE)

  keys->api_key      = mmap(NULL, api_stat.st_size, PROT_READ, MAP_PRIVATE, api_fd, 0);
  keys->private_key  = mmap(NULL, key_stat.st_size, PROT_READ, MAP_PRIVATE, key_fd, 0);

  close(key_fd);
  close(api_fd);
}

static void map_new_files(keys_t *const keys)
{
  const uint16_t key_fd = open(PRIVATE_KEY_PATH, O_WRONLY | O_CREAT, 0600);
  const uint16_t api_fd = open(API_KEY_PATH, O_WRONLY | O_CREAT, 0600);

  ftruncate(key_fd, ED25519_KEY_SIZE);
  ftruncate(api_fd, API_KEY_SIZE);

  struct stat key_stat;
  struct stat api_stat;
  fstat(key_fd, &key_stat);
  fstat(api_fd, &api_stat);

  keys->api_key      = mmap(NULL, api_stat.st_size, PROT_WRITE, MAP_SHARED, api_fd, 0);
  keys->private_key  = mmap(NULL, key_stat.st_size, PROT_WRITE, MAP_SHARED, key_fd, 0);

  close(key_fd);
  close(api_fd);
}

//https://www.wolfssl.com/documentation/manuals/wolfssl/group__ED25519.html
static void generate_keys(const WC_RNG *rng, ed25519_key *const priv_key, ed25519_key *const pub_key)
{
  wc_ed25519_init(priv_key);
  wc_ed25519_make_key(rng, ED25519_KEY_SIZE, priv_key);
  wc_ed25519_make_public(priv_key, pubkey, ED25519_KEY_SIZE);
  wc_ed25519_import_public(pubkey, ED25519_KEY_SIZE, pub_key);
}

static void import_keys(keys_t *const keys, const char *const priv_key_str, const char *const api_key_str)
{
  wc_ed25519_import_private_only(priv_key_str, ED25519_KEY_SIZE, keys->priv_key);
  wc_ed25519_make_public(keys->priv_key, keys->pub_key, ED25519_KEY_SIZE);
  memcpy(keys->priv_key, priv_key_str, ED25519_KEY_SIZE);
}

static void export_keys(const keys_t *const keys, char *const priv_key_str, char *const api_key_str)
{
  //TODO: write keys to files
}

static void unmap_files(char *const priv_key_file_str, char *const api_key_file_str)
{
  munmap(priv_key_file_str, ED25519_KEY_SIZE);
  munmap(api_key_file_str, API_KEY_SIZE);
}

void free_keys(keys_t *const keys)
{
  wc_ed25519_free(keys->priv_key);
  wc_ed25519_free(keys->pub_key);
}