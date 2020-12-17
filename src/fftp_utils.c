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

char *fftp_get_error()
{
	return strerror(errno);
}

void fftp_error(char *message, bool terminate)
{
	fprintf(stdout, "%s (%s)\n", message, fftp_get_error());

	if (terminate)
		exit(0);
}
