/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dns_resolver.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 10:38:46 by craimond          #+#    #+#             */
/*   Updated: 2025/02/05 15:01:36 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DNS_RESOLVER_H
# define DNS_RESOLVER_H

# include <stdint.h>
# include <stdbool.h>
# include <string.h>
# include <arpa/inet.h>
# include <sys/epoll.h>

# include "extensions.h"
# include "errors.h"
# include "message_broker.h"

# define DNS_SERVER "1.1.1.1"
# define MAX_ADDRESSES 3
# define MAX_DOMAIN_LEN 255
# define MAX_LABEL_LEN 63
# define DNS_MAX_PACKET_SIZE 512

# define DNS_FLAG_QR     0x8000
# define DNS_FLAG_OPCODE 0x7800
# define DNS_FLAG_AA     0x0400
# define DNS_FLAG_TC     0x0200
# define DNS_FLAG_RD     0x0100
# define DNS_FLAG_RA     0x0080
# define DNS_FLAG_Z      0x0040
# define DNS_FLAG_AD     0x0020
# define DNS_FLAG_CD     0x0010
# define DNS_FLAG_RCODE  0x000F
# define DNS_QTYPE_A     1
# define DNS_QCLASS_IN   1

typedef struct PACKED {
  const uint16_t id;
  const uint16_t flags;
  const uint16_t qdcount;
  const uint16_t ancount;
  const uint16_t nscount;
  const uint16_t arcount;
} dns_header_t;

typedef struct PACKED {
  char qname[256];
  uint16_t qtype; 
  uint16_t qclass;
} dns_question_t;

typedef struct PACKED {
  const uint16_t name;
  const uint16_t type;
  const uint16_t class;
  const uint32_t ttl;
  const uint16_t rdlength;
  const uint8_t rdata[];
} dns_answer_t;

typedef struct {
  const char *domain;
  uint16_t domain_len;
  uint16_t callback_fd;
} dns_entry_t;

typedef struct {
  uint16_t sock_fd;
  struct sockaddr_in addr;
  dns_entry_t *entries;
  uint16_t n_entries;
} dns_resolver_t;

COLD void init_dns_resolver(dns_resolver_t *restrict resolver);
COLD void resolve_domain(dns_resolver_t *restrict resolver, const char *restrict domain, const uint16_t domain_len, const uint16_t callback_fd);
COLD void handle_dns_responses(const dns_resolver_t *restrict resolver, const uint32_t events);
COLD void free_dns_resolver(const dns_resolver_t *restrict resolver);

#endif