/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:47:42 by craimond          #+#    #+#             */
/*   Updated: 2025/01/11 10:36:46 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

# include <stdint.h>

# define PAIRS = { "usdcust", "fdusdusdt", "usdtdai", "usdpusdt", "fdusdusdc", "eureuri"}
# define LOGGING_INTERVAL 60000U
# define MAX_MARKET_ORDER_DELAY 50U
# define MAX_LIMIT_ORDER_DELAY 1000U

//TODO capire come fare per rendere config const e anche attributi

typedef struct
{
  char    *api_key;
  uint8_t log_level : 2;
} config_t;

void init_config(config_t *const config);

#endif