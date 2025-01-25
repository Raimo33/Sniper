/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dns_resolver.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 10:38:46 by craimond          #+#    #+#             */
/*   Updated: 2025/01/25 18:02:39 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DNS_RESOLVER_H
# define DNS_RESOLVER_H

# include <stdint.h>
# include <stdbool.h>
# include <arpa/inet.h>
# include <sys/epoll.h>

# include "extensions.h"

# define DNS_SERVER "1.1.1.1"
# define DNS_FILENO 8
# define MAX_ADDRESSES 3

typedef struct PACKED {
    uint16_t id;
    uint16_t flags;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
} dns_header_t;

typedef struct PACKED {
  uint16_t qtype;
  uint16_t qclass;
} dns_question_t;

typedef struct PACKED {
  uint16_t type;
  uint16_t class;
  uint32_t ttl;
  uint16_t rdlength;
  // Followed by `rdlength` bytes of data (e.g., IP address).
} dns_rr_t;

typedef struct {
  char *domain;
  uint16_t domain_len;
  struct sockaddr_in *addr;
  uint16_t callback_fd;
} dns_entry_t;

typedef struct {
  struct sockaddr_in addr;
  dns_entry_t entries[MAX_ADDRESSES];
  uint16_t count;
} dns_resolver_t;

void COLD init_dns_resolver(dns_resolver_t *restrict resolver);
bool COLD resolve_domain(dns_resolver_t *restrict resolver, const char *restrict domain, const uint16_t domain_len, struct sockaddr_in *restrict addr, const uint16_t callback_fd);
void COLD handle_dns_responses(const dns_resolver_t *restrict resolver, const uint8_t events);
void COLD free_dns_resolver(const dns_resolver_t *restrict resolver);

#endif