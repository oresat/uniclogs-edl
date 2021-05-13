#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "file.h"
#include "common.h"
#include "frame_buf.h"

ssize_t send_file_seg(FILE *file, char *dest, off_t off, size_t len)
{
    fb_t *frame;
    file_xfr_t *xfr_hdr;
    size_t ret;

    /* Allocate frame and reserve space */
    frame = fb_alloc(FB_MAX_LEN);
    if (frame == NULL) {
        fprintf(stderr, "Error allocating frame: %s\r", strerror(errno));
        return -1;
    }
    fb_reserve(frame, USLP_MAX_HEADER_LEN + sizeof(file_xfr_t));
    frame->data_ptr = fb_put(frame, len);
    if (frame->data_ptr == NULL) {
        fprintf(stderr, "Error reserving space for data\r");
        fb_free(frame);
        return -1;
    }

    /* Seek and copy data */
    fseek(file, off, SEEK_SET);
    ret = fread(frame->data_ptr, 1, len, file);
    if (ret != len) {
        fprintf(stderr, "Error in file read\r");
        fb_free(frame);
        return -1;
    }

    xfr_hdr = fb_push(frame, sizeof(file_xfr_t));
    if (xfr_hdr == NULL) {
        fprintf(stderr, "Error reserving space for payload header\r");
        fb_free(frame);
        return -1;
    }
    memcpy(xfr_hdr->filename, dest, NAME_MAX - 1);
    xfr_hdr->filename[NAME_MAX] = '\0';
    xfr_hdr->off = off;
    xfr_hdr->len = len;

    uslp_map_send(&edl_link, frame, 0, 1, true);

    return ret;
}

size_t send_file(char *src, char *dest)
{
    FILE *file;
    ssize_t len, n = 0;
    off_t off = 0;

    /* Open file */
    file = fopen(src, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s: %s\r", src, strerror(errno));
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    len = ftell(file);

    while (len) {
        n = (len > FILE_BUF_LEN ? FILE_BUF_LEN : len);
        n = send_file_seg(file, dest, off, n);
        if (n < 0) {
            break;
        }
        off += n;
        len -= n;
    }

    fclose(file);
    if (n < 0) {
        exit(EXIT_FAILURE);
    }
    return off;
}
