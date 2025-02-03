/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dns_resolver.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 11:15:29 by craimond          #+#    #+#             */
/*   Updated: 2025/02/03 22:56:47 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/dns_resolver.h"

COLD static void encode_domain(const char *restrict domain, const uint16_t domain_len, char *restrict qname);
COLD static void deserialize_dns_response(const char *restrict buffer, uint16_t *restrict id, uint32_t *restrict ip);

void init_dns_resolver(dns_resolver_t *restrict resolver)
{
  const uint16_t fd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);

  *resolver = (dns_resolver_t){
    .addr = {
      .sin_family = AF_INET,
      .sin_port = htons(53)
    },
    .entries = calloc(MAX_ADDRESSES, sizeof(dns_entry_t)),
  };
  inet_pton(AF_INET, DNS_SERVER, &resolver->addr.sin_addr);
  
  dup2(fd, DNS_FILENO);
  close(fd);
}

void handle_dns_responses(const dns_resolver_t *restrict resolver, const uint8_t events)
{
  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic(STR_AND_LEN("DNS resolver connection error"));

  char buffer[DNS_MAX_PACKET_SIZE];
  uint16_t id;
  uint32_t network_ip;

  while (try_udp_recv(DNS_FILENO, buffer, sizeof(buffer), DNS_SERVER, STR_LEN(DNS_SERVER)))
  {
    deserialize_dns_response(buffer, &id, &network_ip);
    write(resolver->entries[id].callback_fd, &network_ip, sizeof(network_ip));
  }
}

void resolve_domain(dns_resolver_t *restrict resolver, const char *restrict domain, const uint16_t domain_len, const uint16_t callback_fd)
{
  fast_assert(domain, "Unexpected NULL pointer");
  fast_assert(domain_len <= MAX_DOMAIN_LEN, "Domain too long");

  const uint8_t id = resolver->n_entries++;
  resolver->entries[id] = (dns_entry_t) {
    .domain = domain,
    .domain_len = domain_len,
    .callback_fd = callback_fd
  };

  dns_header_t header = (dns_header_t) {
    .id = htons(id),
    .flags = htons(DNS_FLAG_QR | DNS_FLAG_RD),
    .qdcount = htons(1),
    .ancount = 0,
    .nscount = 0,
    .arcount = 0
  };

  dns_question_t question = {
    .qtype = htons(DNS_QTYPE_A),
    .qclass = htons(DNS_QCLASS_IN)
  };
  encode_domain(domain, domain_len, question.qname);

  struct iovec iov[2] = {
    {&header, sizeof(header)},
    {&question, sizeof(question)}
  };

  const struct msghdr msg = {
    .msg_name = &resolver->addr,
    .msg_namelen = sizeof(resolver->addr),
    .msg_iov = iov,
    .msg_iovlen = 2,
    .msg_control = NULL,
    .msg_controllen = 0,
    .msg_flags = 0
  };

  sendmsg(DNS_FILENO, &msg, MSG_DONTWAIT | MSG_NOSIGNAL);
}

static void encode_domain(const char *restrict domain, uint16_t domain_len, char *restrict qname)
{
  uint16_t label_start = 0;
  uint8_t label_len;

  for (uint16_t domain_pos = 0; LIKELY(domain_pos <= domain_len); domain_pos++)
  {
    if (UNLIKELY(domain_pos == domain_len || domain[domain_pos] == '.'))
    {
      label_len = domain_pos - label_start;

      if (UNLIKELY(label_len == 0))
      {
        fast_assert(domain_pos == domain_len, "Invalid domain: empty label");
        break;
      }

      fast_assert(label_len <= MAX_LABEL_LEN, "Invalid domain: label too long");

      *qname++ = label_len;
      memcpy(qname, domain + label_start, label_len);
      qname += label_len;
      label_start = domain_pos + 1;
    }
  }
  
  *qname++ = 0;
}

//TODO check header, check body etc, double check buffer overflows
static void deserialize_dns_response(const char *buffer, uint16_t *restrict id, uint32_t *restrict ip)
{
  const dns_header_t *header = (dns_header_t *)buffer;
  *id = ntohs(header->id);

  fast_assert(header->flags & DNS_FLAG_QR, "Invalid DNS response");
  fast_assert(header->flags & DNS_FLAG_RD, "DNS server does not support recursion");

  buffer += sizeof(dns_header_t);
  while (LIKELY(*buffer != '\0'))
    buffer += *buffer + 1;
  buffer += 1 + sizeof(dns_question_t);

  dns_answer_t *answer;
  uint16_t type, class, rdlength;
  uint8_t ancount = ntohs(header->ancount);
  bool is_compressed;
  while (LIKELY(ancount--))
  {
    is_compressed = (*buffer & 0xC0) == 0xC0;
    buffer += 2 * is_compressed;
    while (UNLIKELY(is_compressed == false && *buffer != '\0'))
      buffer += *buffer + 1;
    buffer += (is_compressed == false);

    answer = (dns_answer_t *)buffer;
    type = ntohs(answer->type);
    class = ntohs(answer->class);
    rdlength = ntohs(answer->rdlength);

    if (LIKELY(type == DNS_QTYPE_A && class == DNS_QCLASS_IN))
    {
      fast_assert(rdlength == sizeof(uint32_t), "Invalid A record length");
      memcpy(ip, answer->rdata, sizeof(uint32_t));
      return;
    }
    buffer += sizeof(dns_answer_t) + rdlength;
  }
  panic(STR_AND_LEN("No valid record in DNS response"));
}

void free_dns_resolver(const dns_resolver_t *restrict resolver)
{
  close(DNS_FILENO);
 
  if (UNLIKELY(resolver == NULL))
    return;

  free(resolver->entries);
}