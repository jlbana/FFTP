#include "fftp_utils.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>

int fftp_file_exists(char * fileName)
{
        return access(fileName, F_OK);
}

char *fftp_get_error()
{
	return strerror(errno);
}
