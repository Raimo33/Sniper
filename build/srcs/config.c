/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:15:41 by craimond          #+#    #+#             */
/*   Updated: 2025/01/10 18:01:13 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/config.h"
#include "headers/logger.h"

void init_config(void)
{
  g_config.api_key = getenv("API_KEY");
  g_config.log_level = map_log_level(getenv("LOG_LEVEL"));
}