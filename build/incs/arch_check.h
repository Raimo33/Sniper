/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arch_check.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 14:31:29 by craimond          #+#    #+#             */
/*   Updated: 2025/02/09 14:32:51 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARCH_CHECK_H
# define ARCH_CHECK_H

#if defined(__arm__) || defined(__x86_64__) || defined(__i386__)
#else
  #error "This architecture does not support unaligned memory access"
#endif

#endif