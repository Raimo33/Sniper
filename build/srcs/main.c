/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:07:42 by craimond          #+#    #+#             */
/*   Updated: 2025/01/08 20:00:51 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//https://developers.binance.com/docs/binance-spot-api-docs/faqs/spot_glossary

#include "headers/arb.h"

t_config g_config = {0};

//TODO error checking per tutte le funzioni con goto
int32_t main(void)
{
  init_config();
  //TODO init epoll loop
}
