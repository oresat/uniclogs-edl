#ifndef _CMD_H_
#define _CMD_H_

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint8_t cmd;
    uint8_t arg[];
} cmd_t;

void send_cmd(uint8_t cmd_code, void *arg, size_t arg_len);

#endif /* _CMD_H_ */
