#include "fftp_utils.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int fftp_file_exists(char * fileName)
{
	// TODO: per-user implementation
        return 1;
}

void fftp_error(char *message, bool terminate)
{
	perror(message);

	if (terminate)
		exit(0);
}
