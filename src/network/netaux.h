#ifndef _NETAUX_H
#define _NETAUX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <sys/epoll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define MAX_LINE         1460
#define BACKLOG          SOMAXCONN
#define HTTP_SERVER_PORT 45555
#define HTTP_BUFFER_SIZE (MAX_LINE * 20)
#define EPOLL_SIZE       1024


/**
 * Aux functions
 */
void ensure_buffer_initialized (void);
int write_line (int fd, const char *line, size_t length);
void write_http_headers (int fd);
void write_http_body (int fd);
void write_http_content (int fd);

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


/**
 * Wrappers
 */
int socket_err (int domain, int type, int protocol);
int setsockopt_err (int sockfd, int level, int optname,
                    const void *optval, socklen_t optlen);
int set_signal_handler_err (int signo, sig_func func);
int bind_err (int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int listen_err (int sockfd, int backlog);
int accept_err (int sockfd, struct sockaddr *addr, socklen_t *addrlen);
pid_t fork_err (void);
int close_err (int fd);
void write_line_err (int fd, const char *line, size_t length);
int select_err (int nfds, fd_set *readfds, fd_set *writefds,
                fd_set *exceptfds, struct timeval *timeout);
int poll_err (struct pollfd *fds, nfds_t nfds, int timeout);
int epoll_create_err (int size);
int epoll_wait_err (int epfd, struct epoll_event *events,
                    int maxevents, int timeout);
void epoll_ctl_err (int epfd, int op, int fd, struct epoll_event *event);

void pthread_create_err (pthread_t *thread, const pthread_attr_t *attr,
                         void *(*start_routine) (void *), void *arg);
void pthread_detach_err (pthread_t thread);
void pthread_mutex_lock_err (pthread_mutex_t *mutex);
void pthread_mutex_unlock_err (pthread_mutex_t *mutex);

#endif /* _NETAUX_H */
