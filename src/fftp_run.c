#include "fftp_run.h"
#include "fftp_init.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;

	switch(key)
	{
		case 'i':
			arguments->ip = arg;
			break;
		case 'p':
		{
			unsigned port = atoi(arg);
			arguments->port = port;
			break;
		}
		default:
			return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

struct argp_option options[] =
{
	{ "port", 'p', "Port", 0, NULL, 0 },
	{ "ip", 'i', "Address", 0, NULL, 0 },
	{ 0 }
};

struct argp argp = { options, parse_opt, NULL, DOC };

int main(int argc, char *argv[])
{
	struct arguments arguments;

	memset(&arguments, 0, sizeof(arguments));
	arguments.port = htons(21);

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	fftp_init(arguments.ip, arguments.port);
	return 0;
}
