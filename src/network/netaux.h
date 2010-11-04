#ifndef _NETAUX_H
#define _NETAUX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#define MAX_LINE 1460
#define BACKLOG  0

void err_quit (const char *where);

#endif /* _NETAUX_H */
