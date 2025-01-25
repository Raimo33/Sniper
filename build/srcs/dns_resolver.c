/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dns_resolver.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 11:15:29 by craimond          #+#    #+#             */
/*   Updated: 2025/01/25 14:02:28 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/dns_resolver.h"

void init_dns_resolver(dns_resolver_t *restrict resolver)
{
  const uint16_t fd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
  resolver->addr = (struct sockaddr_in){
    .sin_family = AF_INET,
    .sin_port = htons(53)
  };
  inet_pton(AF_INET, "1.1.1.1", &resolver->addr.sin_addr);
  
  dup2(fd, DNS_FILENO);
  close(fd);
}

void handle_dns_resolution(const dns_resolver_t *restrict resolver, const char event, struct sockaddr_in *restrict ws_addr, struct sockaddr_in *restrict fix_addr, struct sockaddr_in *restrict rest_addr)
{
  static void *restrict states[] = {&&
  static uint8_t sequence = 0;

  if (UNLIKELY(event == 'e'))
    panic(STR_LEN_PAIR("DNS resolution error"));

  //TODO state machine
}

void free_dns_resolver(const dns_resolver_t *restrict resolver)
{
  close(DNS_FILENO);
}