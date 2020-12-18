#include "fftp_auth.h"

#include <stdio.h>
#include <crypt.h>
#include <shadow.h>
#include <string.h>

bool _fftp_auth_verify(char *username, char *password)
{
	struct spwd *entry = getspnam(username);
	if (entry == NULL)
	{
		return false;
	}

	char *original = entry->sp_pwdp, *result;
	result = crypt(password, original);

	if (! strcmp(result, original))
		return true;
	return false;
}

bool fftp_auth_verify(char *username, char *password)
{
	if (! strncmp(username, "root", 4))
	{
		return false;
	}
	return _fftp_auth_verify(username, password);
}
