/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dns_resolver.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 11:15:29 by craimond          #+#    #+#             */
/*   Updated: 2025/01/25 17:35:34 by craimond         ###   ########.fr       */
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
  inet_pton(AF_INET, DNS_SERVER, &resolver->addr.sin_addr);
  
  dup2(fd, DNS_FILENO);
  close(fd);
}

void handle_dns_responses(const dns_resolver_t *restrict resolver, const uint8_t events)
{
  if (UNLIKELY(events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)))
    panic("DNS resolver connection error");

  //TODO ricezione delle risposte (e' stato ricevuto un EPOLLIN)
  return;
}

bool resolve_domain(dns_resolver_t *restrict resolver, const char *restrict domain, const uint16_t domain_len, struct sockaddr_in *restrict addr, const uint16_t callback_fd)
{
  resolver->entries[resolver->count++] = (dns_entry_t) {domain, domain_len, addr, callback_fd};
  //TODO aggiunge elemento all'array di entry
  //TODO ritorna true se le ha inviate tutte, false se ha bisogno di un altro epollout
  //TODO ID sequenziale che corrisponde all'indice dell'entry
}

static void receive_responses()
{
  //TODO parsing minimo, potrebbero esservi piu risposte, ma non a meta'. all or nothing
  //TODO matching dell'ID con l'entry
  //TODO armare il fd del client con EPOLLOUT
}

void free_dns_resolver(const dns_resolver_t *restrict resolver)
{
  close(DNS_FILENO);
}