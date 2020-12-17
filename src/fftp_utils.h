#ifndef _FFTP_UTILS_H
#define _FFTP_UTILS_H

#include <stdbool.h>

int   fftp_file_exists(char *);
char *fftp_get_error(void);
void  fftp_error(char *, bool);

#endif
