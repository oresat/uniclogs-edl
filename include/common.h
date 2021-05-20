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
#ifndef DEFAULT_TX_MQ
#define DEFAULT_TX_MQ "/uniclogs_tx_queue"
#endif

#define FECF_LEN        2
#define CMD_RESP_LEN    64
#define FILE_BUF_LEN    1024
#define USLP_MAX_LEN    (FB_MAX_LEN + FECF_LEN)

extern uslp_link_t edl_link;
extern uint16_t scid;
extern uint8_t vcid;
extern uint8_t mapid;

void pdu_send(fb_t *frame, void *arg);
void pdu_send_ahead(fb_t *frame, void *arg);
void sdu_recv(fb_t *frame, void *arg);

#endif /* _COMMON_H_ */
