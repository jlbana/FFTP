#ifndef _FFTP_CONN_H
#define _FFTP_CONN_H

#include <netinet/in.h>

struct Connection
{
	int     clientFd;               /* Client Socket Identifier */
	unsigned short nQueries;        /* Query Counter */
	short   dataMode;               /* Passive/Active Transfer */
	struct sockaddr_in clientAddr;  /* Client IPv4 Address */
};

void fftp_log_connection(struct Connection);

#endif
