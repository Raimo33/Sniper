/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   yyjson_primitives.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:25:32 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 13:52:38 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "primitives/yyjson_primitives.h"

# include <stdio.h> //TODO remove

inline yyjson_doc * yyjson_read_fp_p(FILE *fp, yyjson_read_flag flg, const yyjson_alc *alc, yyjson_read_err *err)
{
  yyjson_doc *doc = yyjson_read_fp(fp, flg, alc, err);
  if (doc == NULL)
  {
    printf("DEB YYJSON_READ_FD ERROR\n");
    panic(err->msg);
  }
  return doc;
}