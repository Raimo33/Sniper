/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dns_resolver.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 10:38:46 by craimond          #+#    #+#             */
/*   Updated: 2025/01/26 14:06:56 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DNS_RESOLVER_H
# define DNS_RESOLVER_H

# include <stdint.h>
# include <stdbool.h>
# include <arpa/inet.h>
# include <sys/epoll.h>

# include "extensions.h"
# include "errors.h"

# define DNS_SERVER "1.1.1.1"
# define DNS_FILENO 8
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

typedef struct PACKED {
  uint16_t id;
  uint16_t flags;
  uint16_t qdcount;
  uint16_t ancount;
  uint16_t nscount;
  uint16_t arcount;
} dns_header_t;

typedef struct PACKED {
  char qname[256];
  uint16_t qtype; 
  uint16_t qclass;
} dns_question_t;

typedef struct PACKED {
  uint16_t name;
  uint16_t type;
  uint16_t class;
  uint32_t ttl;
  uint16_t rdlength;
  uint8_t rdata[];
} dns_answer_t;

typedef struct {
  char *domain;
  uint16_t domain_len;
  uint16_t callback_fd;
} dns_entry_t;

typedef struct {
  struct sockaddr_in addr;
  dns_entry_t entries[MAX_ADDRESSES];
  uint16_t count;
} dns_resolver_t;

void COLD init_dns_resolver(dns_resolver_t *restrict resolver);
void COLD resolve_domain(dns_resolver_t *restrict resolver, const char *restrict domain, const uint16_t domain_len, const uint16_t callback_fd);
void COLD handle_dns_responses(const dns_resolver_t *restrict resolver, const uint8_t events);
void COLD free_dns_resolver(const dns_resolver_t *restrict resolver);

#endif