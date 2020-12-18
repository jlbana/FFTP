#ifndef _FFTP_AUTH_H
#define _FFTP_AUTH_H

#define IS_LOGGED(c) ((c)->uid != -1)	/* Client authenticated? */

#include <stdbool.h>

bool fftp_auth_verify(char *, char *);

#endif
