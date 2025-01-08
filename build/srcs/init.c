/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 20:00:57 by craimond          #+#    #+#             */
/*   Updated: 2025/01/08 20:01:45 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/config.h"
#include "headers/logging.h"

void init_config(void)
{
  g_config.api_key = getenv("API_KEY");
  g_config.log_level = map_log_level(getenv("LOG_LEVEL"));
}