#include "fftp_connection.h"
#include "fftp_parse.h"
#include "fftp_auth.h"

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

int fftp_get_count()
{
	return nThreads;
}

void fftp_log_connection(struct Connection *conn)
{
	struct	sockaddr_in *clientAddr = conn->clientAddr;
	char	*ip;
	unsigned short port;

	ip	= inet_ntoa(clientAddr->sin_addr);
	port	= ntohs(clientAddr->sin_port);

	fprintf(fLog, "[%d] Connection from %s:%d\n", conn->clientFd, ip, port);
	fflush(fLog);
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
	fftp_add_count(-1);
	fprintf(fLog, "[%d] Connection terminated\n", conn->clientFd);
	fflush(fLog);

	free(conn->clientAddr);
	free(conn);
}

void *fftp_handle_connection(void *arg)
{
	struct	Connection *conn = arg;
	struct	Command *command = malloc(sizeof(struct Command));
	int	status, clientFd = conn->clientFd;
	char	buf[256], *end;

	dprintf(clientFd, ">> ");

	while( (status = read(
	clientFd,
	buf,
	sizeof buf)) > 0)
	{
		end = strchr(buf, '\n');
		if (end != NULL) *end = 0;

		fftp_parse_command(buf, command);

		if (IS_LOGGED(conn) &&
		( command->operation == 0 || command->operation == 1 ))
		{
			goto resume;
		}

		switch (command->operation)
		{
		case 0:
		{
			char *arg = command->argument;
			dprintf(
			clientFd,
			"USER issued with argument: %s\n",
			arg);

			if ( arg != NULL && strlen(arg))
			{
				if (conn->username)
					free(conn->username);
				conn->username = strdup(arg);
			}
			break;
		}
		case 1:
			dprintf(
			clientFd,
			"PASS issued with argument: %s\n",
			command->argument);

			if (conn->username != NULL && command->argument)
			{
				if(! fftp_auth_verify(
				conn,
				command->argument))
				{
				free(conn->username);
				conn->username = NULL;
				}
			}
			break;
		default:
			dprintf(clientFd, "No idea?\n");
			break;
		}
	resume:
		if (IS_LOGGED(conn))
			dprintf(clientFd, "(%d) >> ", conn->uid);
		else
			dprintf(clientFd, ">> ");
	}

	fftp_free_connection(conn);
	free(command);
	return NULL;
}
