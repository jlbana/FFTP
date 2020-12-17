#include "fftp_init.h"
#include "fftp_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

FILE    *fLog;          /* Handle to Log file */
int     serverFd;       /* Server Socket Identifier */

bool fftp_init (char *ip, unsigned short port)
{
	struct	sockaddr_in serverAddr;
	int	status;

	fLog = fopen(LOG_FILE, "a");

	if (fLog == NULL)
	{
		fftp_error("fopen()", true);
	}

	memset(&serverAddr, 0, sizeof(struct sockaddr_in));
	serverAddr.sin_family	= AF_INET;
	serverAddr.sin_port	= htons(port);

	status = inet_aton(ip, &serverAddr.sin_addr);

	if (status == 0)
	{
		fputs("Incorrect IP supplied", stdout);
		exit(0);
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

	status = bind(
	serverFd,
	(struct sockaddr *) &serverAddr,
	sizeof(serverAddr)
	);

	if ( status == -1)
	{
		fftp_error("bind()", true);
	}

	status = listen(serverFd, NALLOWED);

	if( status == -1)
	{
		fftp_error("listen()", true);
	}


	close(serverFd);
	return true;
}
