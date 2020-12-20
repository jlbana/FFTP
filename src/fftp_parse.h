#ifndef _FFTP_PARSE_H
#define _FFTP_PARSE_H

#define NCMDS	2

#include <stdbool.h>

struct Command
{
	int	operation;
	char	*argument;
};

bool fftp_parse_command(char *, struct Command *);

#endif
