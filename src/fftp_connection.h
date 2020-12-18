#ifndef _FFTP_CONN_H
#define _FFTP_CONN_H

#include <netinet/in.h>

enum dMode {
	DT_PASSIVE = 0,
	DT_ACTIVE,
	DT_16MAX = 0xffff
};

struct Connection
{
	int		clientFd;	/* Client Socket Identifier */
	unsigned short	nQueries;	/* Query Counter */
	enum dMode	dataMode;	/* Passive/Active Transfer */
	struct sockaddr_in *clientAddr;	/* Client IPv4 Address */
	short		uid, gid;	/* Client Permissions */
};

void fftp_log_connection(struct Connection *);
struct Connection *fftp_new_connection();

#endif
