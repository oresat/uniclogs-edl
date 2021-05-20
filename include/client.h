#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <mqueue.h>

extern mqd_t tx_mqd;

void udp_start(char *hostname, char *port);
void udp_stop(void);

#endif /* _CLIENT_H_ */
