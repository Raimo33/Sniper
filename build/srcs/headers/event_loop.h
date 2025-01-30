/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:42:49 by craimond          #+#    #+#             */
/*   Updated: 2025/01/30 21:06:12 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_LOOP_H
# define EVENT_LOOP_H

# include <sys/epoll.h>
# include <stdbool.h>

# include "extensions.h"
# include "signals.h"
# include "ws.h"
# include "fix.h"
# include "rest.h"
# include "dns_resolver.h"
# include "logger.h"

# define MAX_EVENTS 128

typedef struct
{
  const uint16_t epoll_fd;
    // Can add more event loop related state here
} event_loop_ctx_t;

COLD void  init_event_loop(event_loop_ctx_t *restrict ctx);
COLD void  establish_connections(const event_loop_ctx_t *restrict ctx, fix_client_t *fix_client, ws_client_t *ws_client, rest_client_t *rest_client, dns_resolver_t *dns_resolver);
COLD void  listen_events(const event_loop_ctx_t *restrict ctx, const fix_client_t *fix_client, const ws_client_t *ws_client, const rest_client_t *rest_client);
COLD void  free_event_loop(const event_loop_ctx_t *restrict ctx);

#endif