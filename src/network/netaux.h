#ifndef _NETAUX_H
#define _NETAUX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netdb.h>


#define MAX_LINE         1460
#define BACKLOG          SOMAXCONN
#define HTTP_SERVER_PORT 45555


extern char HTTP_BUFFER[MAX_LINE];
void init_buffer (void);


/**
 *  Signals
 */
typedef void sig_func (int);
int set_signal_handler (int signo, sig_func *func);
void sig_wait_child (int signo);


/**
 *  Error handling
 */
void err_quit (const char *where);

#endif /* _NETAUX_H */
