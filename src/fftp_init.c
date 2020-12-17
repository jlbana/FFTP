#include "fftp_init.h"
#include "fftp_utils.h"

#include <stdio.h>
#include <string.h>
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
		fftp_error("Opening log file failed", true);
	}

	memset(&serverAddr, 0, sizeof(struct sockaddr_in));
	serverAddr.sin_family	= AF_INET;
	serverAddr.sin_port	= htons(port);

	status = inet_aton(ip, &serverAddr.sin_addr);

	if (status == 0)
	{
		fftp_error("Incorrect IP supplied", true);
	}

	serverFd = socket(
	AF_INET,
	SOCK_STREAM,
	IPPROTO_TCP
	);

	if (serverFd < 0)
	{
		fftp_error("Socket creation failed", true);
	}

	

	return true;
}
