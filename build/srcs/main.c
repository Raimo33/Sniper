/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:07:42 by craimond          #+#    #+#             */
/*   Updated: 2025/01/06 01:32:56 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//https://developers.binance.com/docs/binance-spot-api-docs/faqs/spot_glossary

#include "headers/arb.h"

//TODO error checking per tutte le funzioni con goto
int32_t main(void)
{
  const char *api_key = getenv("API_KEY");
  const char *ca_certs_path = getenv("CA_CERTS_PATH");

  if (!api_key || !ca_certs_path)
    return (puts_fd("Missing environment variables", STDERR_FILENO), 1);

  //TODO init epoll loop
}
