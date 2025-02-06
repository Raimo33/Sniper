/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_tweaks.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:09:14 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 10:27:35 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSTEM_TWEAKS_H
# define SYSTEM_TWEAKS_H

# include <stdint.h>
# include <sys/resource.h>

# include "primitives/sys_primitives.h"
# include "extensions.h"

# define MAX_FDS 32

COLD void set_fd_limit(const uint16_t limit);

#endif