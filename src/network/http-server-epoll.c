#include "netaux.h"

int main (void) {
  int listenfd, peer, epoll;
  int i, maxevents, nready;
  struct epoll_event ev, events[EPOLL_MAX_EVENTS];

  listenfd = tcp_listen ();

  epoll = epoll_create_err (EPOLL_SIZE);
  ev.events = EPOLLIN;
  ev.data.fd = listenfd;
  epoll_ctl_err (epoll, EPOLL_CTL_ADD, listenfd, &ev);
  maxevents = 1;


  for (;;) {
    nready = epoll_wait_err (epoll, events, maxevents, -1);
    for (i = 0; i < nready; i++) {
      if (events[i].data.fd == listenfd) {
        if (maxevents == EPOLL_MAX_EVENTS) {
          fprintf (stderr, "too many clients");
          exit (EXIT_FAILURE);
        }
        peer = tcp_accept (listenfd);

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
