#ifndef _FILE_H_
#define _FILE_H_

#include <stdio.h>
#include <stdint.h>

#define NAME_MAX 32
#define FILE_BUF_LEN 1024

typedef struct {
    char filename[NAME_MAX];
    off_t off;
    size_t len;
    uint8_t data[];
} file_xfr_t;

ssize_t send_file_seg(FILE *file, char *dest, off_t off, size_t len);
size_t send_file(char *src, char *dest);

#endif
