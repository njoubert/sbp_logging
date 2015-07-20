#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

/** Signed 8-bit integer. */
typedef int8_t s8;
/** Signed 16-bit integer. */
typedef int16_t s16;
/** Signed 32-bit integer. */
typedef int32_t s32;
/** Signed 64-bit integer. */
typedef int64_t s64;
/** Unsigned 8-bit integer. */
typedef uint8_t u8;
/** Unsigned 16-bit integer. */
typedef uint16_t u16;
/** Unsigned 32-bit integer. */
typedef uint32_t u32;
/** Unsigned 64-bit integer. */
typedef uint64_t u64;

/** Return value indicating success. */
#define SBP_OK              0
/** Return value indicating message decoded and callback executed by sbp_process. */
#define SBP_OK_CALLBACK_EXECUTED 1
/** Return value indicating message decoded with no associated callback in sbp_process. */
#define SBP_OK_CALLBACK_UNDEFINED 2
/** Return value indicating an error with the callback (function defined). */
#define SBP_CALLBACK_ERROR -1
/** Return value indicating a CRC error. */
#define SBP_CRC_ERROR      -2
/** Return value indicating an error occured whilst sending an SBP message. */
#define SBP_SEND_ERROR     -3
/** Return value indicating an error occured because an argument was NULL. */
#define SBP_NULL_ERROR     -4

/** State structure for processing SBP messages. */
typedef struct {
  enum {
    WAITING = 0,
    GET_TYPE,
    GET_SENDER,
    GET_LEN,
    GET_MSG,
    GET_CRC
  } state;
  u16 msg_type;
  u16 sender_id;
  u16 crc;
  u8 msg_len;
  u8 n_read;
  u8 msg_buff[256];
} sbp_state_t;

sbp_state_t sbp_state;

u16 crc16_ccitt(const u8 *buf, u32 len, u16 crc);
s8 sbp_process(sbp_state_t *s, u8 temp);
void sbp_process_message(sbp_state_t* s);
