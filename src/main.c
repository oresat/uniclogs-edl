#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "common.h"
#include "client.h"
#include "cmd.h"
#include "file.h"

#define MAXBUF 256

int main(int argc, char *argv[])
{
    char *hostname = "127.0.0.1";
    char *port = "52001";
	int c;
    char buf[MAXBUF];

	/* Command line argument processing */
	while ((c = getopt(argc, argv, "h:p:s:v:m:")) != -1) {
		switch (c) {
        case 'h':
            hostname = optarg;
            break;
        case 'p':
            port = optarg;
            break;
        case 's':
            scid = strtoul(optarg, NULL, 0);
            break;
        case 'v':
            vcid = strtoul(optarg, NULL, 0);
            if (vcid > 31)
                goto usage;
            break;
        case 'm':
            mapid = strtoul(optarg, NULL, 0);
            if (mapid > 15)
                goto usage;
            break;
        case '?':
        default:
            goto usage;
		}
	}

    udp_start(hostname, port);

    while (fgets(buf, MAXBUF, stdin)) {
        char src[256], dst[32];
        uint32_t arg;
        size_t len;

        buf[strcspn(buf, "\n")] = '\0';
        if (!strcmp(buf, "exit")) {
            break;
        } else if (sscanf(buf, "tx %x", &arg) == 1) {
            if (mapid == 16) {
                mapid = DEFAULT_MAPID_CMD;
            }
            send_cmd(0, &arg, 1);
        } else if (sscanf(buf, "upload %255s %31s", src, dst) == 2) {
            if (mapid == 16) {
                mapid = DEFAULT_MAPID_FILE;
            }
            len = send_file(src, dst);
            printf("Sent %lu bytes\n", len);
        }
    }

    udp_stop();

	exit(EXIT_SUCCESS);
usage:
    fprintf(stderr, "Usage: %s [-h <host>] [-p <port>] [-s <scid>] [-v <vcid>] [-m <mapid>]\n", argv[0]);
    exit(EXIT_FAILURE);
}
