#include <string.h>
#include <mqueue.h>

#include "common.h"
#include "command.h"
#include "file.h"

uint16_t scid = DEFAULT_SCID;
uint8_t vcid = DEFAULT_VCID;
uint8_t mapid = 0;

static const uslp_map_t map_cmd = {
    .sdu            = SDU_MAP_ACCESS,
    .upid           = UPID_MAPA_SDU,
    .max_pkt_len    = CMD_RESP_LEN,
    .incomplete     = false,
    .map_recv       = frame_rx,
};

static const uslp_map_t map_file = {
    .sdu            = SDU_MAP_ACCESS,
    .upid           = UPID_MAPA_SDU,
    .max_pkt_len    = FILE_BUF_LEN + sizeof(file_xfr_t),
    .incomplete     = false,
    .map_recv       = frame_rx,
};

static const uslp_vc_t vc0 = {
    .seq_ctrl_len   = 0,
    .expedited_len  = 0,
    .seq_ctrl_cnt   = NULL,
    .expedited_cnt  = NULL,
    .cop            = COP_NONE,
    .mapid[0]       = &map_cmd,
    .mapid[1]       = &map_file,
    .trunc_tf_len   = USLP_MAX_LEN,
    .ocf            = false,
};

static uslp_mc_t mc = {
    .vcid[0]        = &vc0,
};

static const uslp_pc_t pc = {
    .name           = "EDL Physical Channel",
    .tf_len         = USLP_MAX_LEN,
    .fecf           = FECF_HW,
    .fecf_len       = FECF_LEN,
    .phy_send       = frame_tx,
    .phy_send_prio  = frame_tx,
};

uslp_link_t edl_link = {
    .mc = &mc,
    .pc_rx = &pc,
    .pc_tx = &pc,
};

void link_init(void)
{
    mc.scid = scid;
}

void frame_tx(fb_t *frame)
{

}

void frame_rx(fb_t *frame_rx)
{

}

fb_t *__fb_alloc(void) {
    fb_t *fb = malloc(sizeof(fb_t));
    memset(fb, 0, sizeof(fb_t));
    return fb;
}

void __fb_free(fb_t *fb) {
    free(fb);
}

void __fb_post(void *queue, fb_t *fb) {
    mqd_t mqd = *((mqd_t*)queue);
    mq_send(mqd, (char*)fb, sizeof(fb_t), 0);
}

void __fb_post_first(void *queue, fb_t *fb) {
    mqd_t mqd = *((mqd_t*)queue);
    mq_send(mqd, (char*)fb, sizeof(fb_t), 1);
}

fb_t *__fb_get(void *queue) {
    fb_t *fb = NULL;
    return fb;
}
