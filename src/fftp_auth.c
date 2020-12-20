#include "fftp_auth.h"

#include <pwd.h>
#include <stdio.h>
#include <crypt.h>
#include <shadow.h>
#include <string.h>

bool _fftp_auth_verify(struct Connection *conn, char *password)
{
	struct spwd *entry = getspnam(conn->username);

	if (entry == NULL)
	{
		return false;
	}

	char *original = entry->sp_pwdp, *result;
	result = crypt(password, original);
	memset(password, 0, strlen(password));

	if (! strcmp(result, original))
	{
		struct passwd *record = getpwnam(conn->username);
		conn->uid = record->pw_uid;
		conn->gid = record->pw_gid;
		return true;
	}
	return false;
}

bool fftp_auth_verify(struct Connection *conn, char *password)
{
	if (! strncmp(conn->username, "root", 4))
	{
		return false;
	}

	return _fftp_auth_verify(conn, password);
}
