#include "netaux.h"

int main (void) {
  int i, maxevents;
  int listenfd, peer, epoll;
  int nready;
  struct sockaddr_in servaddr, peeraddr;
  socklen_t len;
  int on = 1;
  struct epoll_event ev, *events;
  struct rlimit rl;
  rlim_t openmax = 1024;

  if (getrlimit (RLIMIT_NOFILE, &rl) != -1)
    openmax = rl.rlim_max;
  events = (struct epoll_event *) malloc (sizeof (struct epoll_event) * openmax);

  listenfd = socket_err (AF_INET, SOCK_STREAM, 0);
  setsockopt_err (listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on));

  bzero (&servaddr, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
  servaddr.sin_port = htons (HTTP_SERVER_PORT);

  bind_err (listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
  listen_err (listenfd, BACKLOG);

  epoll = epoll_create_err (EPOLL_SIZE);
  ev.events = EPOLLIN;
  ev.data.fd = listenfd;
  epoll_ctl_err (epoll, EPOLL_CTL_ADD, listenfd, &ev);
  maxevents = 1;

  for (;;) {
    nready = epoll_wait_err (epoll, events, maxevents, -1);
    for (i = 0; i < nready; i++) {
      if (events[i].data.fd == listenfd) {
        len = sizeof (peeraddr);
        peer = accept_err (listenfd, (struct sockaddr *) &peeraddr, &len);

        ev.events = EPOLLOUT;
        ev.data.fd = peer;
        epoll_ctl_err (epoll, EPOLL_CTL_ADD, peer, &ev);
        maxevents++;
      }
      else {
        peer = events[i].data.fd;
        write_http_content (peer);
        close_err (peer);
        maxevents--;
      }
    }
  }

  return 0;
}
