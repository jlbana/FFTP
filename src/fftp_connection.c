#include "fftp_connection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <arpa/inet.h>

extern FILE	*fLog;

pthread_mutex_t	countLock;	/* Mutex for TC */
unsigned int	nThreads = 0;	/* Thread count */

static void fftp_dec_count()
{
	pthread_mutex_lock(&countLock);
	nThreads--;
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


	while( (status = read(clientFd,
	buf,
	sizeof(buf)) >= 0)
	{

	}

	fftp_free_connection(conn);
	fftp_dec_count();
	return NULL;
}
