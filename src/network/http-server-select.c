#include "netaux.h"

int main (void) {
  int listenfd, peer;
  int i, maxi, maxfd;
  int nready, client[FD_SETSIZE];
  fd_set rset, wset, persistent;

  listenfd = tcp_listen ();

  maxfd = listenfd;
  maxi = -1;
  for (i = 0; i < FD_SETSIZE; i++)
    client[i] = -1;

  FD_ZERO (&rset);
  FD_ZERO (&persistent);

  for (;;) {
    FD_SET (listenfd, &rset);
    wset = persistent;

    nready = select_err (maxfd + 1, &rset, &wset, NULL, NULL);

    if (FD_ISSET (listenfd, &rset)) {
      peer = tcp_accept (listenfd);

      for (i = 0; i < FD_SETSIZE; i++)
        if (client[i] < 0) {
          client[i] = peer;
          break;
        }

      if (i == FD_SETSIZE) {
        fprintf (stderr, "too many clients");
        exit (EXIT_FAILURE);
      }

      FD_SET (peer, &persistent);
      if (peer > maxfd)
        maxfd = peer;
      if (i > maxi)
        maxi = i;

      if (--nready <= 0)
        continue;
    }

    for (i = 0; i <= maxi; i++) {
      if ((peer = client[i]) < 0)
        continue;

      write_http_content (peer);
      close_err (peer);
      FD_CLR (peer, &persistent);
      client[i] = -1;

      if (--nready <= 0)
        break;
    }
  }

  return 0;
}
