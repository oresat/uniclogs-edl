#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

#include "client.h"
#include "common.h"

int sd = 0;
mqd_t tx_mqd = 0;
pthread_t rx_thd, tx_thd;
volatile bool terminate = false;

fb_t *queue_fetch(void *arg)
{
    fb_t *frame = NULL;
    mqd_t mqd = *((mqd_t*)arg);
    if (mq_receive(mqd, (char*)&frame, sizeof(frame), NULL) < 0) {
        fprintf(stderr, "Error fetching frame from queue: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return frame;
}

void *udp_rx(void *arg)
{
    struct addrinfo *p = arg;
    (void)p;
    fb_t *fb;
    ssize_t len;

    while (!terminate) {
        if ((fb = fb_alloc(FB_MAX_LEN, NULL)) == NULL) {
            continue;
        }
        if ((len = recvfrom(sd, fb->data, FB_MAX_LEN, 0, NULL, NULL)) < 0) {
			fprintf(stderr, "Error: Receive failure: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
        } else if (len == 0) {
            fb_free(fb, NULL);
            continue;
        }
        fb_put(fb, len);
        uslp_recv(&edl_link, fb);
        fb_free(fb, NULL);
    }

    pthread_exit(NULL);
}

void *udp_tx(void *arg)
{
    struct addrinfo *p = arg;
    fb_t *fb;
    ssize_t len;

    while (!terminate) {
        if ((fb = queue_fetch(&tx_mqd)) == NULL) {
            continue;
        }
        if ((len = sendto(sd, fb->data, fb->len, 0, p->ai_addr, p->ai_addrlen)) < 0) {
			fprintf(stderr, "Error: Send failure: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
        }
        fb_free(fb, NULL);
    }

    pthread_exit(NULL);
}

void udp_start(char *hostname, char *port)
{
    int ret;
    struct addrinfo hints, *servinfo, *p;
    struct mq_attr mq_attr = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = sizeof(fb_t*),
    };

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((ret = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        exit(EXIT_FAILURE);
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
			fprintf(stderr, "Error: Cannot create socket: %s\n", strerror(errno));
			continue;
        }
        break;
    }

    if (p == NULL) {
		fprintf(stderr, "Error: Socket failed\n");
		exit(EXIT_FAILURE);
    }

    tx_mqd = mq_open(DEFAULT_TX_MQ, O_CREAT | O_RDWR, 0644, &mq_attr);
    if (tx_mqd < 0) {
        fprintf(stderr, "Error opening message queue: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    pthread_create(&rx_thd, NULL, udp_rx, p);
    pthread_create(&tx_thd, NULL, udp_tx, p);

    freeaddrinfo(servinfo);
    return;
}

void udp_stop(void)
{
    terminate = true;
    pthread_join(rx_thd, NULL);
    pthread_join(tx_thd, NULL);
    mq_close(tx_mqd);
    close(sd);
}
