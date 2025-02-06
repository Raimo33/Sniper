/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_primitives.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:01:26 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 10:05:21 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALLOC_PRIMITIVES_H
# define ALLOC_PRIMITIVES_H

# include <stdint.h>
# include <stdlib.h>

# include "extensions.h"
# include "errors.h"

HOT extern inline void	*malloc_p(size_t size);
HOT extern inline void	*calloc_p(size_t nmemb, size_t size);
HOT extern inline void	*realloc_p(void *ptr, size_t size);

#endif