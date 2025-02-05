/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fastaf.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 14:15:47 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 15:27:03 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FASTAF_H
# define FASTAF_H

# include "logger.h"
# include "errors.h"
# include "signals.h"
# include "fix.h"
# include "ws.h"
# include "rest.h"
# include "dns_resolver.h"
# include "event_loop.h"
# include "keys.h"
# include "fast_ssl.h"
# include "graph.h"

typedef struct
{
  keys_t keys;
  SSL_CTX *ssl_ctx;
  uint16_t epoll_fd;
  clients_t clients;
  uint16_t log_fd;
  uint16_t sig_fd;
  graph_t graph;
} app_resources_t;

//https://developers.binance.com/docs/binance-spot-api-docs/faqs/spot_glossary

//TODO SSL_shutdown() //attenzione al non-blocking

#endif