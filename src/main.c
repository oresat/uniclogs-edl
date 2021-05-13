#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "common.h"
#include "command.h"
#include "file.h"

bool verbose_flag = false;
op_t operation = OP_NONE;

int main(int argc, char *argv[])
{
    char *filename = NULL;
    uint8_t cmd = 0;
	int c;

	/* Command line argument processing */
	while ((c = getopt(argc, argv, "c:f:v")) != -1) {
		switch (c) {
        case 'c':
            cmd = strtoul(optarg, NULL, 0);
            break;
        case 'f':
            filename = optarg;
            break;
        case 'v':
            verbose_flag = true;
            break;
        case '?':
        default:
            fprintf(stderr, "Usage: %s -c <cmd_code> -f <filename> [-v]\n", argv[0]);
            exit(EXIT_FAILURE);
		}
	}

	exit(EXIT_SUCCESS);
}
