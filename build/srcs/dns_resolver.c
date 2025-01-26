/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dns_resolver.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 11:15:29 by craimond          #+#    #+#             */
/*   Updated: 2025/01/26 13:17:55 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/dns_resolver.h"

static void encode_domain(const char *restrict domain, const uint16_t domain_len, char *restrict qname);
static void parse_dns_response(const uint8_t *restrict buffer, const uint16_t len, uint16_t *restrict id, char *restrict ip);

void init_dns_resolver(dns_resolver_t *restrict resolver)
{
  const uint16_t fd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
  resolver->addr = (struct sockaddr_in){
    .sin_family = AF_INET,
    .sin_port = htons(53)
  };
  inet_pton(AF_INET, DNS_SERVER, &resolver->addr.sin_addr);
  
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
  uint16_t len;
  uint16_t id;
  uint32_t resolved_ip;

  while (true)
  {
    len = recvfrom(DNS_FILENO, buffer, DNS_MAX_PACKET_SIZE, MSG_DONTWAIT, &src_addr, &src_addr_len);
    if (UNLIKELY(len < 0))
      break;

    inet_ntop(AF_INET, &src_addr.sin_addr, src_ip, INET_ADDRSTRLEN);
    assert(strcmp(src_ip, DNS_SERVER) == 0, STR_LEN_PAIR("Invalid DNS server response"));

    parse_dns_response(buffer, len, &id, &resolved_ip);
    write(resolver->entries[id].callback_fd, &resolved_ip, sizeof(resolved_ip));
  }
}

void resolve_domain(dns_resolver_t *restrict resolver, const char *restrict domain, const uint16_t domain_len, const uint16_t callback_fd)
{
  assert(domain_len <= MAX_DOMAIN_LEN, STR_LEN_PAIR("Domain too long"));

  const uint8_t id = resolver->count++;
  resolver->entries[id] = (dns_entry_t) {domain, domain_len, callback_fd};

  const dns_header_t header = (dns_header_t) {
    .id = htons(id),
    .flags = htons(DNS_FLAG_QR_QUERY | DNS_FLAG_RD),
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

  const struct iovec iov[2] = {
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

  assert(domain_len <= MAX_DOMAIN_LEN, STR_LEN_PAIR("Domain too long"));

  for (uint16_t domain_pos = 0; LIKELY(domain_pos <= domain_len); domain_pos++)
  {
    if (UNLIKELY(domain_pos == domain_len || domain[domain_pos] == '.'))
    {
      label_len = domain_pos - label_start;

      if (UNLIKEYLY(label_len == 0))
      {
        assert(domain_pos == domain_len, STR_LEN_PAIR("Invalid domain: empty label"));
        break;
      }

      assert(label_len <= MAX_LABEL_LEN, STR_LEN_PAIR("Invalid domain: label too long"));

      *qname++ = label_len;
      memcpy(qname, domain + label_start, label_len);
      qname += label_len;
      label_start = domain_pos + 1;
    }
  }
  
  *qname++ = 0;
}

static void parse_dns_response(const uint8_t *restrict buffer, const uint16_t len, uint16_t *restrict id, char *restrict ip)
{
  //TODO
}

void free_dns_resolver(const dns_resolver_t *restrict resolver)
{
  close(DNS_FILENO);
}