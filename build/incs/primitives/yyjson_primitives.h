/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   yyjson_primitives.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:25:43 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 11:26:22 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef YYJSON_PRIMITIVES_H
# define YYJSON_PRIMITIVES_H

# include <stdint.h>
# include <yyjson.h>

# include "extensions.h"
# include "errors.h"

HOT extern inline yyjson_doc *yyjson_read_fp_p(FILE *fp, size_t flags, size_t *err_line, size_t *err_col);

#endif