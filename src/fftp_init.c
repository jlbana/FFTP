#include "fftp_init.h"
#include "fftp_utils.h"

#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

FILE    *fLog;          /* Handle to Log file */
int     serverFd;       /* Server Socket Identifier */

bool fftp_init (char *ip, unsigned short port)
{
	struct sockaddr_in serverAddr;

	fLog = fopen(LOG_FILE, "a");

	memset(serverAddr, 0, sizeof(struct sockaddr_in));
	inet_aton(ip, &serverAddr.sin_addr);
	serverAddr.sin_port     = htons(port);
	serverAddr.sin_family   = AF_INET;

	serverFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	return true;
}
