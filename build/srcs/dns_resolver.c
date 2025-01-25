/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dns_resolver.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 11:15:29 by craimond          #+#    #+#             */
/*   Updated: 2025/01/25 22:03:24 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/dns_resolver.h"

static void encode_domain(const char *restrict domain, const uint16_t domain_len, char *restrict qname);

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
    panic("DNS resolver connection error");

  //TODO ricezione delle risposte (e' stato ricevuto un EPOLLIN)
  //TODO parsing minimo, potrebbero esservi piu risposte, ma non a meta'. all or nothing
  //TODO matching dell'ID con l'entry
  //TODO armare il fd del client con EPOLLOUT
  return;
}

void resolve_domain(dns_resolver_t *restrict resolver, const char *restrict domain, const uint16_t domain_len, struct sockaddr_in *restrict addr, const uint16_t callback_fd)
{
  assert(domain_len <= 255, STR_LEN_PAIR("Domain too long"));

  const uint8_t id = resolver->count++;
  resolver->entries[id] = (dns_entry_t) {domain, domain_len, addr, callback_fd};

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

  sendmsg(DNS_FILENO, &msg, 0);
}

static void encode_domain(const char *restrict domain, uint16_t domain_len, char *restrict qname)
{
  uint16_t label_start = 0;
  uint8_t label_len;

  assert(domain_len <= 255, STR_LEN_PAIR("Domain too long"));

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

      assert(label_len <= 63, STR_LEN_PAIR("Invalid domain: label too long"));

      *qname++ = label_len;
      memcpy(qname, domain + label_start, label_len);
      qname += label_len;
      label_start = domain_pos + 1;
    }
  }
  
  *qname++ = 0;
}

void free_dns_resolver(const dns_resolver_t *restrict resolver)
{
  close(DNS_FILENO);
}