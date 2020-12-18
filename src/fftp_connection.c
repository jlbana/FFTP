#include "fftp_connection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <arpa/inet.h>

extern FILE	*fLog;

pthread_mutex_t	countLock;	/* Mutex for TC */
int		nThreads = 0;	/* Thread count */

void fftp_add_count(int value)
{
	pthread_mutex_lock(&countLock);
	nThreads += value;
	pthread_mutex_unlock(&countLock);
}

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

void fftp_free_connection(struct Connection *conn)
{
	free(conn->clientAddr);
	free(conn);
}

void *fftp_handle_connection(void *arg)
{
	struct	Connection *conn = arg;
	int	status, clientFd = conn->clientFd;
	char	buf[256];

	while( (status = read(
	clientFd,
	buf,
	sizeof buf)) > 0)
	{
		// Parsing
	}

	fftp_free_connection(conn);
	fftp_add_count(-1);
	return NULL;
}
