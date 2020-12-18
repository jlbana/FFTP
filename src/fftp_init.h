#ifndef _FFTP_INIT_H
#define _FFTP_INIT_H

#define LOG_FILE	"fftp.log"
#define NMAXQUEUE	4

#include <stdbool.h>

bool fftp_init(char *, unsigned short);

#endif
