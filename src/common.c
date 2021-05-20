#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "common.h"
#include "client.h"
#include "cmd.h"
#include "file.h"

uint16_t scid = DEFAULT_SCID;
uint8_t vcid = DEFAULT_VCID;
uint8_t mapid = 16;

static const uslp_map_t map_cmd = {
    .sdu            = SDU_MAP_ACCESS,
    .upid           = UPID_MAPA_SDU,
    .max_pkt_len    = CMD_RESP_LEN,
    .incomplete     = false,
    .map_recv       = sdu_recv,
};

static const uslp_map_t map_file = {
    .sdu            = SDU_MAP_ACCESS,
    .upid           = UPID_MAPA_SDU,
    .max_pkt_len    = FILE_BUF_LEN + sizeof(file_xfr_t),
    .incomplete     = false,
    .map_recv       = sdu_recv,
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
    .scid           = DEFAULT_SCID,
    .vcid[0]        = &vc0,
};

static const uslp_pc_t pc = {
    .name           = "EDL Physical Channel",
    .tf_len         = USLP_MAX_LEN,
    .fecf           = FECF_HW,
    .fecf_len       = FECF_LEN,
    .phy_send       = pdu_send,
    .phy_send_ahead = pdu_send_ahead,
    .send_arg       = &tx_mqd,
    .send_ahead_arg = &tx_mqd,
};

uslp_link_t edl_link = {
    .mc = &mc,
    .pc_rx = &pc,
    .pc_tx = &pc,
};

void pdu_send(fb_t *frame, void *arg)
{
    mqd_t mqd = *((mqd_t*)arg);
    if (mq_send(mqd, (char*)&frame, sizeof(frame), 0) < 0) {
        fprintf(stderr, "Error sending frame to queue: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void pdu_send_ahead(fb_t *frame, void *arg)
{
    mqd_t mqd = *((mqd_t*)arg);
    if (mq_send(mqd, (char*)&frame, sizeof(frame), 1) < 0) {
        fprintf(stderr, "Error sending frame to queue: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void sdu_recv(fb_t *frame, void *arg)
{
    printf("Received %lu byte response:\n", frame->len);
    for (size_t i = 0; i < frame->len; i++) {
        if ((i % 0x10) == 0) printf("\n%08lX:", i);
        printf(" %02X", frame->data[i]);
    }
    printf("\n");
}

fb_t *__fb_alloc(size_t len, void *arg) {
    (void)len;
    (void)arg;
    fb_t *fb = malloc(sizeof(fb_t));
    memset(fb, 0, sizeof(fb_t));
    return fb;
}

void __fb_free(fb_t *fb, void *arg) {
    (void)arg;
    free(fb);
}
