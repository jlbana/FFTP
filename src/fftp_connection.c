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

	fftp_message(clientFd, 220, "FFTP");
	while( (status = read(
	clientFd,
	buf,
	sizeof buf)) > 0)
	{
		end	= strchr(buf, '\r');
		if (end != NULL)   *end = 0;

		fftp_parse_command(buf, command);

		if (IS_LOGGED(conn) &&
		( command->operation == 0 || command->operation == 1 ))
		{
			fftp_message(clientFd, 530, "Already logged in.");
			continue;
		}

		switch (command->operation)
		{
		case 0:
		{
			char *arg = command->argument;

			if ( arg != NULL && strlen(arg))
			{
				if (conn->username)
					free(conn->username);
				conn->username = strdup(arg);
			}

			fftp_message(clientFd, 331, "Waiting for password.");
			break;
		}
		case 1:

			if (conn->username != NULL && command->argument)
			{
				if(! fftp_auth_verify(
				conn,
				command->argument))
				{
					free(conn->username);
					conn->username = NULL;
				}
				else
				{
					fftp_message(clientFd, 230, "Authentication successful.");
					break;
				}
			}

			fftp_message(clientFd, 530, "Authentication failed.");
			break;
		case 2:
			fftp_message(clientFd, 221, "Goodbye!");
			goto end;
		default:
			fftp_message(clientFd, 500, "No idea.");
			break;
		}
	}

end:
	fftp_free_connection(conn);
	free(command);
	return NULL;
}
