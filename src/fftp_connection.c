#include "fftp_connection.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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

struct Connection *fftp_new_connection()
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	memset(conn, 0, sizeof(*conn));

	conn->uid = -1;
	conn->gid = -1;
	conn->clientAddr = malloc(sizeof(struct sockaddr_in));
	return conn;
}

