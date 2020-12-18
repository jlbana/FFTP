#include "fftp_init.h"
#include "fftp_utils.h"
#include "fftp_connection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

FILE    *fLog;          /* Handle to Log file */
int     serverFd;       /* Server Socket Identifier */

extern unsigned int nThreads;

void fftp_deinit (int signal)
{
	fputs("Terminating..", stdout);
	close(serverFd);
	exit(EXIT_SUCCESS);
}

void fftp_loop(void)
{
	struct	Connection *conn;
	int	clientFd, szStruct;
	pthread_t th;

	while (true)
	{
		if (nThreads > 4)
			continue;

		conn = fftp_new_connection();
		szStruct = sizeof(struct sockaddr_in);

		clientFd = accept(
		serverFd,
		(struct sockaddr *) conn->clientAddr,
		&szStruct);

		if (clientFd == -1)
		{
			fftp_free_connection(conn);
			continue;
		}

		fftp_log_connection(conn);
		fftp_add_count(1);

		pthread_create(
		&th,
		NULL,
		fftp_handle_connection,
		conn);
	}
}

void fftp_init (char *ip, unsigned short port)
{
	struct	sockaddr_in serverAddr;
	struct	sigaction action;

	fLog = fopen(LOG_FILE, "a");

	if (fLog == NULL)
	{
		fftp_error("fopen()", true);
	}

	memset(&serverAddr, 0, sizeof(struct sockaddr_in));
	serverAddr.sin_family	= AF_INET;
	serverAddr.sin_port	= htons(port);

	if( inet_aton(ip, &serverAddr.sin_addr) == 0)
	{
		fputs("Incorrect IP supplied", stdout);
		exit(EXIT_FAILURE);
	}

	serverFd = socket(
	AF_INET,
	SOCK_STREAM,
	IPPROTO_TCP
	);

	if (serverFd < 0)
	{
		fftp_error("socket()", true);
	}

	if (bind(
	serverFd,
	(struct sockaddr *) &serverAddr,
	sizeof(serverAddr)) == -1)
	{
		fftp_error("bind()", true);
	}

	if (listen(serverFd, NMAXQUEUE) == -1)
	{
		fftp_error("listen()", true);
	}

	memset(&action, 0, sizeof action);
	action.sa_handler = fftp_deinit;
	sigaction(SIGINT, &action, NULL);

	fftp_loop();
}
