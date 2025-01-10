/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:07:42 by craimond          #+#    #+#             */
/*   Updated: 2025/01/10 17:53:02 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//https://developers.binance.com/docs/binance-spot-api-docs/faqs/spot_glossary

#include "headers/arb.h"

t_config g_config = {0};

//TODO error checking per tutte le funzioni con goto
int32_t main(void)
{
  init_config();
  init_logger();
  init_signals();
  init_ws();
  init_fix();
  init_rest();
  init_event_loop();
  start_event_loop();
  cleanup();
}
