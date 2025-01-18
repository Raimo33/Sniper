/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_buffers.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 10:52:14 by craimond          #+#    #+#             */
/*   Updated: 2025/01/18 10:53:12 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHARED_BUFFERS_H
# define SHARED_BUFFERS_H

# define SEND_BUFFER_SIZE 4096U
# define RECV_BUFFER_SIZE 4096U

# include "extensions.h"

extern char send_buffer[SEND_BUFFER_SIZE] ALIGNED(16);
extern char recv_buffer[RECV_BUFFER_SIZE] ALIGNED(16);

#endif