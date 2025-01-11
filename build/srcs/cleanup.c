/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:36:59 by craimond          #+#    #+#             */
/*   Updated: 2025/01/11 16:37:40 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void cleanup(event_loop_ctx_t *const loop, fix_client_t *const fix, ws_client_t *const ws, rest_client_t *const rest)
{
  wolfSSL_Cleanup();
  free_fix(fix);
  free_ws(ws);
  free_rest(rest);
  close(loop->epoll_fd); //forse non basta
}
