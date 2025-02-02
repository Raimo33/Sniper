/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix_parser.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 15:34:10 by craimond          #+#    #+#             */
/*   Updated: 2025/02/02 15:36:19 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIX_PARSER_H
# define FIX_PARSER_H

# include <stdint.h>

# define MSG_TYPE_HEARTBEAT "0"
# define MSG_TYPE_TEST_REQUEST "1"
# define MSG_TYPE_REJECT "3"
# define MSG_TYPE_LOGOUT "5"
# define MSG_TYPE_EXECUTION_REPORT "8"
# define MSG_TYPE_ORDER_CANCEL_REJECT "9"
# define MSG_TYPE_LOGON "A"
# define MSG_TYPE_NEWS "B"
# define MSG_TYPE_NEW_ORDER_SINGLE "D"
# define MSG_TYPE_NEW_ORDER_LIST "E"
# define MSG_TYPE_ORDER_CANCEL_REQUEST "F"
# define MSG_TYPE_LIST_STATUS "N"
# define MSG_TYPE_ORDER_MASS_CANCEL_REQUEST "q"
# define MSG_TYPE_ORDER_MASS_CANCEL_REPORT "r"
# define MSG_TYPE_ORDER_CANCEL_REQUEST_AND_NEW_ORDER_SINGLE "XCN"
# define MSG_TYPE_LIMIT_QUERY "XLQ"
# define MSG_TYPE_LIMIT_RESPONSE "XLR"

# define ORD_STATUS_NEW "0"
# define ORD_STATUS_PARTIALLY_FILLED "1"
# define ORD_STATUS_FILLED "2"
# define ORD_STATUS_CANCELED "4"
# define ORD_STATUS_PENDING_CANCEL "6"
# define ORD_STATUS_REJECTED "8"
# define ORD_STATUS_PENDING_NEW "A"
# define ORD_STATUS_EXPIRED "C"

# define ORD_TYPE_MARKET "1"
# define ORD_TYPE_LIMIT "2"
# define ORD_TYPE_STOP "3"
# define ORD_TYPE_STOP_LIMIT "4"

# define SIDE_BUY "1"
# define SIDE_SELL "2"

# define TIME_IN_FORCE_GOOD_TILL_CANCEL "1"
# define TIME_IN_FORCE_IMMEDIATE_OR_CANCEL "3"
# define TIME_IN_FORCE_FILL_OR_KILL "4"

# define EXEC_INST_PARTICIPATE_DONT_INITIATE "6"

# define SESSION_REJECT_REASON_INVALID_TAG_NUMBER "0"
# define SESSION_REJECT_REASON_REQUIRED_TAG_MISSING "1"
# define SESSION_REJECT_REASON_TAG_NOT_DEFINED "2"
# define SESSION_REJECT_REASON_UNDEFINED_TAG "3"
# define SESSION_REJECT_REASON_INCORRECT_VALUE "5"
# define SESSION_REJECT_REASON_INCORRECT_DATA_FORMAT "6"
# define SESSION_REJECT_REASON_SIGNATURE_PROBLEM "8"
# define SESSION_REJECT_REASON_SENDINGTIME_ACCURACY_PROBLEM "10"
# define SESSION_REJECT_REASON_DUPLICATE_TAG "13"
# define SESSION_REJECT_REASON_TAG_OUT_OF_ORDER "14"
# define SESSION_REJECT_REASON_REPEATING_GROUP_ORDER "15"
# define SESSION_REJECT_REASON_INCORRECT_GROUP_COUNT "16"
# define SESSION_REJECT_REASON_OTHER "99"

# define AGGRESSOR_INDICATOR_YES "Y"
# define AGGRESSOR_INDICATOR_NO "N"

# define SELF_TRADE_PREVENTION_NONE "1"
# define SELF_TRADE_PREVENTION_EXPIRE_TAKER "2"
# define SELF_TRADE_PREVENTION_EXPIRE_MAKER "3"
# define SELF_TRADE_PREVENTION_EXPIRE_BOTH "4"

# define MASS_CANCEL_TYPE_SYMBOL "1"
# define MASS_CANCEL_RESPONSE_REJECTED "0"
# define MASS_CANCEL_RESPONSE_ACCEPTED "1"
# define MASS_CANCEL_REJECT_REASON_OTHER "99"

# define TRIGGER_TYPE_PRICE_MOVEMENT "4"
# define TRIGGER_ACTION_ACTIVATE "1"
# define TRIGGER_PRICE_TYPE_LAST_TRADE "2"
# define TRIGGER_DIRECTION_UP "U"
# define TRIGGER_DIRECTION_DOWN "D"

# define ORD_REJ_REASON_OTHER "99"

# define CONTINGENCY_TYPE_ONE_CANCELS_OTHER "1"
# define CONTINGENCY_TYPE_ONE_TRIGGERS_OTHER "2"

# define EXEC_TYPE_NEW "0"
# define EXEC_TYPE_CANCELED "4"
# define EXEC_TYPE_REPLACED "5"
# define EXEC_TYPE_REJECTED "8"
# define EXEC_TYPE_TRADE "F"
# define EXEC_TYPE_EXPIRED "C"

# define RESET_SEQ_NUM_YES "Y"
# define RESET_SEQ_NUM_NO "N"

//TODO funzioni di parsing. entrata e uscita??
//TODO aggiungere components e groups?

#endif