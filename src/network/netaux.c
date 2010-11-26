#include "netaux.h"

char HTTP_BUFFER[HTTP_BUFFER_SIZE];
pthread_mutex_t HTTP_BUFFER_MUTEX = PTHREAD_MUTEX_INITIALIZER;

void ensure_buffer_initialized (void) {
  static int once_run = 0;
  int i;

  pthread_mutex_lock_err (&HTTP_BUFFER_MUTEX) ;

  if (once_run) {
    pthread_mutex_unlock_err (&HTTP_BUFFER_MUTEX);
    return;
  }

  for (i = 0; i < HTTP_BUFFER_SIZE - 2; i++)
    HTTP_BUFFER[i] = 'a';
  HTTP_BUFFER[HTTP_BUFFER_SIZE - 2 - 1] = '\r';
  HTTP_BUFFER[HTTP_BUFFER_SIZE - 1 - 1] = '\n';
  once_run = 1;

  pthread_mutex_unlock_err (&HTTP_BUFFER_MUTEX);
}

int set_signal_handler (int signo, sig_func func) {
  struct sigaction act, old_act;

  act.sa_handler = func;
  sigemptyset (&act.sa_mask);
  act.sa_flags = 0;
  if (signo != SIGALRM)
    act.sa_flags |= SA_RESTART;
  return sigaction (signo, &act, &old_act);
}

void sig_wait_child (int signo __attribute__ ((unused))) {
  pid_t pid;
  int stat;
  while ((pid = waitpid (-1, &stat, WNOHANG)) > 0)
    ;
  return;
}

void err_quit (const char *where) {
  perror (where);
  exit (EXIT_FAILURE);
}

int write_line (int fd, const char *line, size_t length) {
  size_t sent = 0;
  size_t left = length;
  int n = 0;

  while ((n = write (fd, line + sent, left)) >= 0) {
    sent += n;
    left -= n;
    if (left <= 0)
      break;
  }
  if (n < 0)
    return n;
  return sent;
}

int socket_err (int domain, int type, int protocol) {
  int sockfd;

  sockfd = socket (domain, type, protocol);
  if (sockfd < 0)
    err_quit ("socket");

  return sockfd;
}

int setsockopt_err (int sockfd, int level, int optname,
                    const void *optval, socklen_t optlen)
{
  int ret;

  ret = setsockopt (sockfd, level, optname, optval, optlen);
  if (ret != 0)
    err_quit ("setsockopt");

  return ret;
}

int set_signal_handler_err (int signo, sig_func func) {
  int ret;

  ret = set_signal_handler (signo, func);
  if (ret < 0)
    err_quit ("sigaction");

  return ret;
}

int bind_err (int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  int ret;

  ret = bind (sockfd, addr, addrlen);
  if (ret < 0)
    err_quit ("bind");

  return ret;
}

int listen_err (int sockfd, int backlog) {
  int ret;

  ret = listen (sockfd, backlog);
  if (ret != 0)
    err_quit ("listen");

  return ret;
}

int accept_err (int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
  int peer;

  for (;;) {
    peer = accept (sockfd, addr, addrlen);
    if (peer < 0) {
      if (errno == EINTR)
        continue;
      else
        err_quit ("accept");
    }

    return peer;
  }
}

pid_t fork_err (void) {
  pid_t childpid;

  childpid = fork ();
  if (childpid < 0)
    err_quit ("fork");

  return childpid;
}

int close_err (int fd) {
  int ret;

  ret = close (fd);
  if (ret != 0)
    err_quit ("close");

  return ret;
}

void write_line_err (int fd, const char *line, size_t length) {
  int ret;

  ret = write_line (fd, line, length);
  if (ret < 0)
    err_quit ("write");
}

void write_http_headers (int fd) {
  char line[MAX_LINE + 1];

  snprintf (line, sizeof (line), "HTTP/1.1 200 OK\r\n");
  write_line_err (fd, line, strlen (line));
  snprintf (line, sizeof (line), "Content-Length: %d\r\n", HTTP_BUFFER_SIZE);
  write_line_err (fd, line, strlen (line));
  snprintf (line, sizeof (line), "Connection: close\r\n");
  write_line_err (fd, line, strlen (line));
  snprintf (line, sizeof (line), "Content-Type: text/plain; charset=UTF-8\r\n");
  write_line_err (fd, line, strlen (line));
}

void write_http_body (int fd) {
  ensure_buffer_initialized ();
  write_line_err (fd, HTTP_BUFFER, HTTP_BUFFER_SIZE);
}

void write_http_content (int fd) {
  write_http_headers (fd);
  write_line_err (fd, "\r\n", strlen ("\r\n"));
  write_http_body (fd);
}

void pthread_create_err (pthread_t *thread, const pthread_attr_t *attr,
                         void *(*start_routine) (void *), void *arg)
{
  if (pthread_create (thread, attr, start_routine, arg) != 0)
    exit (EXIT_FAILURE);
}

void pthread_detach_err (pthread_t thread) {
  if (pthread_detach (thread) != 0)
    exit (EXIT_FAILURE);
}

void pthread_mutex_lock_err (pthread_mutex_t *mutex) {
  if (pthread_mutex_lock (mutex) != 0)
    exit (EXIT_FAILURE);
}

void pthread_mutex_unlock_err (pthread_mutex_t *mutex) {
  if (pthread_mutex_unlock (mutex) != 0)
    exit (EXIT_FAILURE);
}

int select_err (int nfds, fd_set *readfds, fd_set *writefds,
                fd_set *exceptfds, struct timeval *timeout)
{
  int nready;

  if ((nready = select (nfds, readfds, writefds, exceptfds, timeout)) < 0)
    err_quit ("select");

  return nready;
}

int poll_err (struct pollfd *fds, nfds_t nfds, int timeout) {
  int nready;

  if ((nready = poll (fds, nfds, timeout)) < 0)
    err_quit ("poll");

  return nready;
}

int epoll_create_err (int size) {
  int epfd;

  if ((epfd = epoll_create (size)) < 0)
    err_quit ("epoll_create");

  return epfd;
}

int epoll_wait_err (int epfd, struct epoll_event *events,
                    int maxevents, int timeout)
{
  int nready;

  if ((nready = epoll_wait (epfd, events, maxevents, timeout)) < 0)
    err_quit ("epoll_wait");

  return nready;
}

void epoll_ctl_err (int epfd, int op, int fd, struct epoll_event *event) {
  if (epoll_ctl (epfd, op, fd, event) < 0)
    err_quit ("epoll_ctl");
}

int tcp_listen (void) {
  int listenfd, on = 1;
  struct sockaddr_in servaddr;

  listenfd = socket_err (AF_INET, SOCK_STREAM, 0);
  setsockopt_err (listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on));

  bzero (&servaddr, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
  servaddr.sin_port = htons (HTTP_SERVER_PORT);

  bind (listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
  listen_err (listenfd, BACKLOG);

  return listenfd;
}

int tcp_accept (int listenfd) {
  int peer;
  struct sockaddr_in peeraddr;
  socklen_t len;

  len = sizeof (peeraddr);
  peer = accept_err (listenfd, (struct sockaddr *) &peeraddr, &len);
  return peer;
}
