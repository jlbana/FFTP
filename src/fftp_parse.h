#ifndef _FFTP_PARSE_H
#define _FFTP_PARSE_H

#include <stdbool.h>

struct Command
{
	int	operation;
	char	*arguments;
};

bool fftp_parse_command(char *, struct Command *);

#endif
