/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix_serializer.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <claudio.raimondi@pm.me>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 15:34:10 by craimond          #+#    #+#             */
/*   Updated: 2025/02/03 19:39:21 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIX_PARSER_H
# define FIX_PARSER_H

# include <stdint.h>
# include <stdbool.h>

# define FIX_MESSAGE_HANDLING_UNORDERED 1
# define FIX_MESSAGE_HANDLING_SEQUENTIAL 2
# define FIX_RESPONSE_MODE_EVERYTHING 1
# define FIX_RESPONSE_MODE_ONLY_ACKS 2
# define FIX_MSG_TYPE_LOGON "A"

# define MAX_COMP_ID_SIZE 8
# define MAX_BEGIN_STRING_SIZE 7
# define MAX_MSG_TYPE_SIZE 3
# define MAX_COMP_ID_SIZE 8
# define CHECKSUM_SIZE 3

//TODO riordinare variabili nelle stuct in base a dimensione? farlo ovunque?
//TODO const dove possibile
//TODO ALIGNED??

typedef struct
{
  char *begin_string;
  uint8_t begin_string_len;
  uint32_t body_length;
  char *msg_type;
  uint8_t msg_type_len;
  char *sender_comp_id;
  uint8_t sender_comp_id_len;
  char *target_comp_id;
  uint8_t target_comp_id_len;
  uint64_t msg_seq_num;
  uint64_t sending_time;
  uint16_t recv_window;
} fix_header_t;

typedef struct
{
  uint8_t encrypt_method;
  uint8_t heart_bt_int;
  uint16_t raw_data_length;
  struct 
  {
    uint8_t msg_type;
    char *sender_comp_id;
    uint8_t sender_comp_id_len;
    char *target_comp_id;
    uint8_t target_comp_id_len;
    uint32_t msg_seq_num;
    uint64_t sending_time;
  } raw_data;
  bool reset_seq_num_flag;
  char *username;
  uint8_t username_len;
  uint8_t message_handling;
  uint8_t response_mode;
} fix_logon_t;

typedef struct
{
  char checksum[CHECKSUM_SIZE];
} fix_trailer_t;

typedef struct
{
  fix_header_t header;
  union
  {
    fix_logon_t logon;
    // fix_logout_t logout;
    // fix_heartbeat_t heartbeat;
    //TODO altri messaggi
  };
  fix_trailer_t trailer;
} fix_message_t;

HOT uint16_t serialize_fix_message(char *restrict buffer, const uint16_t buffer_size, const fix_message_t *restrict message);
//TODO deserialize

#endif