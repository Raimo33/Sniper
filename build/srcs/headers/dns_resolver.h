/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dns_resolver.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 10:38:46 by craimond          #+#    #+#             */
/*   Updated: 2025/01/25 11:53:36 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DNS_RESOLVER_H
# define DNS_RESOLVER_H

# include <stdint.h>
# include <arpa/inet.h>

# include "extensions.h"

# define DNS_FILENO 8

// typedef struct {
//   uint16_t id;
//   uint16_t flags;
//   uint16_t qdcount;
//   uint16_t ancount;
//   uint16_t nscount;
//   uint16_t arcount;
// } dns_header_t;

// typedef struct {
//   uint16_t qtype;
//   uint16_t qclass;
// } dns_question_t;

typedef struct {
  struct sockaddr_in addr;
} dns_resolver_t;

void COLD init_dns_resolver(dns_resolver_t *restrict resolver);
void HOT handle_dns_resolution(const dns_resolver_t *restrict resolver, const char event, struct sockaddr_in *restrict ws_addr, struct sockaddr_in *restrict fix_addr, struct sockaddr_in *restrict rest_addr);
void COLD free_dns_resolver(const dns_resolver_t *restrict resolver);

#endif