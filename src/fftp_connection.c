#include "fftp_connection.h"

#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>

extern FILE	*fLog;

void fftp_log_connection(struct Connection *conn)
{
	struct	sockaddr_in *clientAddr = conn->clientAddr;
	char	*ip;
	unsigned short port;

	ip	= inet_ntoa(clientAddr->sin_addr);
	port	= ntohs(clientAddr->sin_port);

	fprintf(fLog, "Connection from %s:%d\n", ip, port);
}
