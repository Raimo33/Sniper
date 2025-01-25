/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:42:49 by craimond          #+#    #+#             */
/*   Updated: 2025/01/25 15:24:56 by craimond         ###   ########.fr       */
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
    uint16_t epoll_fd;
    // Can add more event loop related state here
} event_loop_ctx_t;

COLD void  init_event_loop(event_loop_ctx_t *restrict ctx);
COLD void  establish_connections(const event_loop_ctx_t *restrict ctx, const fix_client_t *fix, const ws_client_t *ws, const rest_client_t *rest);
COLD void  listen_events(const event_loop_ctx_t *restrict ctx, const fix_client_t *fix, const ws_client_t *ws, const rest_client_t *rest);
COLD void  free_event_loop(const event_loop_ctx_t *restrict ctx);

#endif