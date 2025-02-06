/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_primitives.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:08:00 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 10:08:56 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYS_PRIMITIVES_H
# define SYS_PRIMITIVES_H

# include <stdint.h>
# include <sys/resource.h>

# include "extensions.h"
# include "errors.h"

COLD extern inline int32_t setrlimit_p(int32_t resource, const struct rlimit *rlim);

#endif