/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@protonmail.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 18:15:34 by craimond          #+#    #+#             */
/*   Updated: 2025/01/01 18:18:20 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

static size_t puts_fd(const char *s, int fd);

void err(const char *s)
{
  puts_fd(s, STDERR_FILENO);
}

static size_t puts_fd(const char *s, int fd)
{
  return write(fd, s, strlen(s));
}