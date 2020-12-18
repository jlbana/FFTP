#include "fftp_init.h"
#include "fftp_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

FILE    *fLog;          /* Handle to Log file */
int     serverFd;       /* Server Socket Identifier */

void fftp_deinit (int signal)
{
	fputs("Terminating..", stdout);
	close(serverFd);
	exit(EXIT_SUCCESS);
}

void fftp_loop(void)
{
	int clientFd;
	int szStruct;

	while (true)
	{
		struct Connection *conn = new_fftp_connection();
		szStruct = sizeof(struct sockaddr_in);

		clientFd = accept(
		serverFd,
		(struct sockaddr *) conn->clientAddr,
		&szStruct);


		break;
	}
}

bool fftp_init (char *ip, unsigned short port)
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

	return true;
}
