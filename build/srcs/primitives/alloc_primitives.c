/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_primitives.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 09:59:38 by craimond          #+#    #+#             */
/*   Updated: 2025/02/06 10:28:48 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "primitives/alloc_primitives.h"

# include <stdio.h> //TODO remove

inline void *malloc_p(size_t size)
{
  void *ptr = malloc(size);
  if (ptr == NULL)
  {
    printf("DEB MALLOC ERROR\n");
    panic(strerror(errno));
  }
  return ptr;
}

inline void *calloc_p(size_t nmemb, size_t size)
{
  void *ptr = calloc(nmemb, size);
  if (ptr == NULL)
  {
    printf("DEB CALLOC ERROR\n");
    panic(strerror(errno));
  }
  return ptr;
}

inline void *realloc_p(void *ptr, size_t size)
{
  void *new_ptr = realloc(ptr, size);
  if (new_ptr == NULL)
  {
    printf("DEB REALLOC ERROR\n");
    panic(strerror(errno));
  }
  return new_ptr;
}