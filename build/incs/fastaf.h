/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fastaf.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 14:15:47 by craimond          #+#    #+#             */
/*   Updated: 2025/02/09 14:31:17 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FASTAF_H
# define FASTAF_H

# include "arch_check.h"
# include "logger.h"
# include "errors.h"
# include "signals.h"
# include "clients/fix.h"
# include "clients/ws.h"
# include "clients/http.h"
# include "event_loop.h"
# include "keys.h"
# include "fast_ssl.h"
# include "graph.h"

typedef struct
{
  keys_t keys;
  SSL_CTX *ssl_ctx;
  uint8_t epoll_fd;
  clients_t clients;
  uint8_t log_fd;
  uint8_t sig_fd;
  graph_t graph;
} app_resources_t;

//https://developers.binance.com/docs/binance-spot-api-docs/faqs/spot_glossary

//TODO SSL_shutdown() //attenzione al non-blocking

#endif