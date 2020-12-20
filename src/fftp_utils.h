#ifndef _FFTP_UTILS_H
#define _FFTP_UTILS_H

#include <stdbool.h>

int	fftp_file_exists(char *);
void	fftp_error(char *, bool);
void	fftp_message(int, int, char *);

#endif
