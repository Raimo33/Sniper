/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_loop.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:42:49 by craimond          #+#    #+#             */
/*   Updated: 2025/01/11 10:21:44 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_LOOP_H
# define EVENT_LOOP_H

# include <sys/epoll.h>
# include <stdbool.h>

# include "signals.h"
# include "ws.h"
# include "fix.h"
# include "rest.h"
# include "logger.h"

# define MAX_EVENTS 128U

typedef struct {
    uint8_t epoll_fd;
    // Can add more event loop related state here
} event_loop_ctx_t;

void  init_event_loop(event_loop_ctx_t *const ctx, const ws_client_t *const ws, const fix_client_t *const fix, const rest_client_t *const rest);
void  start_event_loop(const event_loop_ctx_t *const ctx, const ws_client_t *const ws, const fix_client_t *const fix, const rest_client_t *const rest);

#endif