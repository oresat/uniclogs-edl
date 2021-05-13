#ifndef _COMMON_H_
#define _COMMON_H_

#include "uslp.h"

#ifndef DEFAULT_SCID
#define DEFAULT_SCID 0x4F53U
#endif
#ifndef DEFAULT_VCID
#define DEFAULT_VCID 0U
#endif
#ifndef DEFAULT_MAPID_CMD
#define DEFAULT_MAPID_CMD 0U
#endif
#ifndef DEFAULT_MAPID_FILE
#define DEFAULT_MAPID_FILE 1U
#endif

#define FECF_LEN        2
#define CMD_RESP_LEN    64
#define FILE_BUF_LEN    1024
#define USLP_MAX_LEN    (FB_MAX_LEN + FECF_LEN)

typedef enum {
    OP_NONE = 0,
    OP_CMD,
    OP_FILE,
} op_t;

extern uslp_link_t edl_link;
extern uint16_t scid;
extern uint8_t vcid;
extern uint8_t mapid;

void link_init(void);
void frame_tx(fb_t *frame);
void frame_rx(fb_t *frame);

#endif
