#include "fftp_parse.h"

#include <string.h>

char commands[NCMDS + 1][6] =
{
	"USER",
	"PASS",
	"QUIT",
	{ 0 }
};

bool fftp_parse_command(char *input, struct Command *command)
{
	int	i;
	char	*separator;

	memset(command, 0, sizeof(*command));
	command->operation = -1;

	separator = strchr(input, ' ');
	if (separator != NULL)
	{
		command->argument = separator + 1;
		*separator = 0;
	}

	for(i = 0; i < NCMDS; i++)
	{
		if (! strcmp(input, commands[i]))
		{
			command->operation = i;
			return true;
		}
	}

	return false;
}
