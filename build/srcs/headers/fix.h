/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:52:51 by craimond          #+#    #+#             */
/*   Updated: 2025/01/25 17:38:38 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIX_H
# define FIX_H

# include <stdint.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <netdb.h>

# include "extensions.h"
# include "ssl.h"
# include "http_parser.h"
# include "shared_buffers.h"

# define FIX_HOST "fix-oe.binance.com"
# define FIX_PORT 9000
# define FIX_FILENO 4

//https://developers.binance.com/docs/binance-spot-api-docs/fix-api#message-components
//https://github.com/binance/binance-spot-api-docs/blob/master/fix-api.md

# define FIX_TAG_BEGINSTRING 8
# define FIX_TAG_BODYLENGTH 9
# define FIX_TAG_MSGTYPE 35
# define FIX_TAG_SENDERCOMPID 49
# define FIX_TAG_TARGETCOMPID 56
# define FIX_TAG_MSGSEQNUM 34
# define FIX_TAG_SENDINGTIME 52
# define FIX_TAG_RECVWINDOW 25000
# define FIX_TAG_CHECKSUM 10
# define FIX_TAG_TESTREQID 112
# define FIX_TAG_ENCRYPTMETHOD 98
# define FIX_TAG_HEARTBTINT 108
# define FIX_TAG_RAWDATALENGTH 95
# define FIX_TAG_RAWDATA 96
# define FIX_TAG_SYMBOL 55
# define FIX_TAG_SIDE 54
# define FIX_TAG_ORDERQTY 38
# define FIX_TAG_PRICE 44
# define FIX_TAG_ORDTYPE 40
# define FIX_TAG_TIMEINFORCE 59
# define FIX_TAG_EXECTYPE 150
# define FIX_TAG_ORDSTATUS 39
# define FIX_TAG_CUMQTY 14
# define FIX_TAG_LEAVESQTY 151
# define FIX_TAG_SELFTRADEPREVENTIONMODE 25001
# define FIX_TAG_CANCELRESTRICTIONS 25002
# define FIX_TAG_LIMITTYPE 25004
# define FIX_TAG_LIMITCOUNT 25005
# define FIX_TAG_LIMITMAX 25006
# define FIX_TAG_LIMITRESETINTERVAL 25007
# define FIX_TAG_LIMITRESETINTERVALRESOLUTION 25008

# define FIX_MSGTYPE_LOGON "A"
# define FIX_MSGTYPE_HEARTBEAT "0"
# define FIX_MSGTYPE_TESTREQUEST "1"
# define FIX_MSGTYPE_REJECT "3"
# define FIX_MSGTYPE_LOGOUT "5"
# define FIX_MSGTYPE_NEWS "B"
# define FIX_MSGTYPE_NEWORDERSINGLE "D"
# define FIX_MSGTYPE_EXECUTIONREPORT "8"
# define FIX_MSGTYPE_ORDERCANCELREQUEST "F"
# define FIX_MSGTYPE_ORDERCANCELREJECT "9"
# define FIX_MSGTYPE_ORDERCANCELREQUESTANDNEWORDERSINGLE "XCN"
# define FIX_MSGTYPE_ORDERMASSCANCELREQUEST "q"
# define FIX_MSGTYPE_ORDERMASSCANCELREPORT "r"
# define FIX_MSGTYPE_NEWORDERLIST "E"
# define FIX_MSGTYPE_LISTSTATUS "N"
# define FIX_MSGTYPE_LIMITQUERY "XLQ"
# define FIX_MSGTYPE_LIMITRESPONSE "XLR"
# define FIX_MSGTYPE_INSTRUMENTLISTREQUEST "x"
# define FIX_MSGTYPE_INSTRUMENTLIST "y"
# define FIX_MSGTYPE_MARKETDATAREQUESTREJECT "Y"
# define FIX_MSGTYPE_MARKETDATASNAPSHOT "W"
# define FIX_MSGTYPE_MARKETDATAINCREMENTALREFRESH "X"
# define FIX_MSGTYPE_NEWORDERLIST "E"
# define FIX_MSGTYPE_LISTSTATUS "N"
# define FIX_MSGTYPE_LIMITQUERY "XLQ"
# define FIX_MSGTYPE_LIMITRESPONSE "XLR"
# define FIX_MSGTYPE_NEWS "B"

# define FIX_ORDTYPE_MARKET '1'
# define FIX_ORDTYPE_LIMIT '2'
# define FIX_ORDTYPE_STOP '3'
# define FIX_ORDTYPE_STOP_LIMIT '4'

# define FIX_SIDE_BUY '1'
# define FIX_SIDE_SELL '2'

# define FIX_TIMEINFORCE_GTC '1'
# define FIX_TIMEINFORCE_IOC '3'
# define FIX_TIMEINFORCE_FOK '4'

# define FIX_EXECTYPE_NEW '0'
# define FIX_EXECTYPE_CANCELED '4'
# define FIX_EXECTYPE_REPLACED '5'
# define FIX_EXECTYPE_REJECTED '8'
# define FIX_EXECTYPE_TRADE 'F'

# define FIX_ORDSTATUS_NEW '0'
# define FIX_ORDSTATUS_PARTIALLY_FILLED '1'
# define FIX_ORDSTATUS_FILLED '2'
# define FIX_ORDSTATUS_CANCELED '4'
# define FIX_ORDSTATUS_PENDING_CANCEL '6'
# define FIX_ORDSTATUS_REJECTED '8'

//TODO aggiungere components e groups

//TODO detect binance decimal values representation on each pair and use fixed point arithmetic

typedef struct
{
  struct sockaddr_in addr;
  SSL *ssl;
  keys_t *keys;
} fix_client_t;

void COLD init_fix(fix_client_t *restrict client, const keys_t *restrict keys, const WOLFSSL_CTX *restrict ssl_ctx);
inline bool HOT handle_fix_connection(const fix_client_t *restrict client, const uint8_t events, const dns_resolver_t *restrict resolver);
void COLD free_fix(const fix_client_t *restrict client);

#endif
