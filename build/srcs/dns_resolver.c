/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dns_resolver.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 11:15:29 by craimond          #+#    #+#             */
/*   Updated: 2025/02/01 21:58:53 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/dns_resolver.h"

COLD static void encode_domain(const char *restrict domain, const uint16_t domain_len, char *restrict qname);
COLD static void parse_dns_response(const uint8_t *restrict buffer, uint16_t *restrict id, uint32_t *restrict ip);

void init_dns_resolver(dns_resolver_t *restrict resolver)
{
  const uint16_t fd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
  resolver->addr = (struct sockaddr_in){
    .sin_family = AF_INET,
    .sin_port = htons(53)
  };
  inet_pton(AF_INET, DNS_SERVER, &resolver->addr.sin_addr);

  resolver->entries = calloc(MAX_ADDRESSES, sizeof(dns_entry_t));
  
  dup2(fd, DNS_FILENO);
  close(fd);
}

void handle_dns_responses(const dns_resolver_t *restrict resolver, const uint8_t events)
{
  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic(STR_LEN_PAIR("DNS resolver connection error"));

  uint8_t buffer[DNS_MAX_PACKET_SIZE];
  struct sockaddr_in src_addr;
  socklen_t src_addr_len = sizeof(src_addr);
  char src_ip[INET_ADDRSTRLEN];
  int16_t len;
  uint16_t id;
  uint32_t resolved_ip;

  while (true)
  {
    len = recvfrom(DNS_FILENO, buffer, DNS_MAX_PACKET_SIZE, MSG_DONTWAIT, (struct sockaddr *)&src_addr, &src_addr_len);
    if (UNLIKELY(len < 0))
      break;

    inet_ntop(AF_INET, &src_addr.sin_addr, src_ip, INET_ADDRSTRLEN);
    fast_assert(memcmp(src_ip, STR_LEN_PAIR(DNS_SERVER)) == 0, STR_LEN_PAIR("Invalid DNS server response"));

    parse_dns_response(buffer, &id, &resolved_ip);
    write(resolver->entries[id].callback_fd, &resolved_ip, sizeof(resolved_ip));
  }
}

void resolve_domain(dns_resolver_t *restrict resolver, const char *restrict domain, const uint16_t domain_len, const uint16_t callback_fd)
{
  fast_assert(domain_len <= MAX_DOMAIN_LEN, STR_LEN_PAIR("Domain too long"));

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

  fast_assert(domain_len <= MAX_DOMAIN_LEN, STR_LEN_PAIR("Domain too long"));

  for (uint16_t domain_pos = 0; LIKELY(domain_pos <= domain_len); domain_pos++)
  {
    if (UNLIKELY(domain_pos == domain_len || domain[domain_pos] == '.'))
    {
      label_len = domain_pos - label_start;

      if (UNLIKELY(label_len == 0))
      {
        fast_assert(domain_pos == domain_len, STR_LEN_PAIR("Invalid domain: empty label"));
        break;
      }

      fast_assert(label_len <= MAX_LABEL_LEN, STR_LEN_PAIR("Invalid domain: label too long"));

      *qname++ = label_len;
      memcpy(qname, domain + label_start, label_len);
      qname += label_len;
      label_start = domain_pos + 1;
    }
  }
  
  *qname++ = 0;
}

//TODO check header, check body etc
static void parse_dns_response(const uint8_t *buf, uint16_t *restrict id, uint32_t *restrict ip)
{
  const dns_header_t *header = (dns_header_t *)buf;
  *id = ntohs(header->id);

  fast_assert(header->flags & DNS_FLAG_QR, STR_LEN_PAIR("Invalid DNS response"));
  fast_assert(header->flags & DNS_FLAG_RD, STR_LEN_PAIR("DNS server does not support recursion"));

  buf += sizeof(dns_header_t);
  while (LIKELY(*buf != '\0'))
    buf += *buf + 1;
  buf += 1 + sizeof(dns_question_t);

  dns_answer_t *answer;
  uint16_t type, class, rdlength;
  uint8_t ancount = ntohs(header->ancount);
  while (LIKELY(ancount--))
  {
    bool is_compressed = (*buf & 0xC0) == 0xC0;
    buf += 2 * is_compressed;
    while (UNLIKELY(is_compressed == false && *buf != '\0'))
      buf += *buf + 1;
    buf += (is_compressed == false);

    answer = (dns_answer_t *)buf;
    type = ntohs(answer->type);
    class = ntohs(answer->class);
    rdlength = ntohs(answer->rdlength);

    if (LIKELY(type == DNS_QTYPE_A && class == DNS_QCLASS_IN))
    {
      fast_assert(rdlength == sizeof(uint32_t), STR_LEN_PAIR("Invalid A record length"));
      memcpy(ip, answer->rdata, sizeof(uint32_t));
      return;
    }
    buf += sizeof(dns_answer_t) + rdlength;
  }
  panic(STR_LEN_PAIR("No valid record in DNS response"));
}

void free_dns_resolver(const dns_resolver_t *restrict resolver)
{
  free(resolver->entries);
  close(DNS_FILENO);
}