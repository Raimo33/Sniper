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

#ifndef FIX_SERIALIZER_H
# define FIX_SERIALIZER_H

# include <stdint.h>
# include <stdbool.h>
# include <immintrin.h>

# include "extensions.h"
# include "errors.h"

# define FIX_BEGINSTRING "8"
# define FIX_BODYLENGTH "9"
# define FIX_MSGTYPE "35"
# define FIX_SENDERCOMPID "49"
# define FIX_TARGETCOMPID "56"
# define FIX_MSGSEQNUM "34"
# define FIX_SENDINGTIME "52"
# define FIX_RECVWINDOW "25000"
# define FIX_RAWDATA "96"
# define FIX_CHECKSUM "10"
# define FIX_TESTREQID "112"
# define FIX_REFSEQNUM "45"
# define FIX_USERNAME "553"
# define FIX_REFTAGID "371"
# define FIX_REFMSGTYPE "372"
# define FIX_SESSIONREJECTREASON "373"
# define FIX_ERRORCODE "25016"
# define FIX_TEXT "58"
# define FIX_MESSAGEHANDLING "25035"
# define FIX_RESPONSEMODE "25036"
# define FIX_DROPCOPYFLAG "9406"
# define FIX_UUID "25037"
# define FIX_ENCRYPTMETHOD "98"
# define FIX_HEARTBTINT "108"
# define FIX_RAWDATALENGTH "95"
# define FIX_RESETSEQNUMFLAG "141"

# define FIX_MSG_TYPE_LOGON "A"

# define FIX_VERSION "FIX.4.4"

typedef struct
{
  const char *tag;
  uint16_t tag_len;
  const char *value;
  uint16_t value_len;
} fix_field_t;

HOT uint16_t serialize_fix_fields(char *restrict buffer, const uint16_t buffer_size, const fix_field_t *restrict fields, const uint16_t n_fields);
HOT uint16_t finalize_fix_message(char *restrict buffer, const uint16_t buffer_size, const uint16_t len);

#endif