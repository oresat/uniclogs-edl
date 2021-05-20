#include <string.h>
#include "cmd.h"
#include "common.h"

void send_cmd(uint8_t cmd_code, void *arg, size_t arg_len)
{
    cmd_t *cmd;
    fb_t *frame = fb_alloc(FB_MAX_LEN, NULL);
    fb_reserve(frame, USLP_MAX_HEADER_LEN);
    cmd = fb_put(frame, sizeof(cmd_t) + arg_len);
    cmd->cmd = cmd_code;
    if (arg != NULL)
        memcpy(cmd->arg, arg, arg_len);
    uslp_map_send(&edl_link, frame, vcid, mapid, true);
}
