/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@protonmail.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:07:42 by craimond          #+#    #+#             */
/*   Updated: 2025/01/01 17:38:47 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//https://developers.binance.com/docs/binance-spot-api-docs/faqs/spot_glossary

#include "headers/arb.h"

//TODO error checking per tutte le funzioni
int32_t main(void)
{
  const char *api_key = getenv("API_KEY");
  const char *ca_certs_path = getenv("CA_CERTS_PATH");

  if (!api_key || !ca_certs_path)
    return (puts_fd("Missing environment variables", STDERR_FILENO), 1);

  uv_loop_t loop;

  uv_loop_init(&loop);
  init_timers(&loop);

  CURL curl_context;
  struct lws wsi;
  //TODO qualcosa per fix

  init_connection(&curl_context, &ws_client, api_key, ca_certs_path, &loop);

  uv_run(loop, UV_RUN_DEFAULT);
  uv_stop(loop);
  cleanup(curl_context, ws_client, &loop);
  return 0;
}
