/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@protonmail.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 17:24:23 by craimond          #+#    #+#             */
/*   Updated: 2025/01/01 17:24:26 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/arb.h"

void cleanup(CURL *curl_context, struct lws *ws_client, uv_loop_t *loop)
{
  curl_easy_cleanup(curl_context);
  lws_context_destroy(ws_client);
  curl_global_cleanup();
  uv_loop_close(loop);
}