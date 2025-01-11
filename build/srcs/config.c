/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:15:41 by craimond          #+#    #+#             */
/*   Updated: 2025/01/11 10:37:03 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/config.h"
#include "headers/logger.h"

void init_config(config_t *const config)
{
  config->api_key = getenv("API_KEY");
  config->log_level = map_log_level(getenv("LOG_LEVEL"));
}