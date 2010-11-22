#include "netaux.h"

int main (void) {
  int i, maxi, maxfd, listenfd, peer;
  int nready, client[FD_SETSIZE];
  struct sockaddr_in servaddr, peeraddr;
  socklen_t len;
  int on = 1;
  fd_set rset, allset;

  listenfd = socket_err (AF_INET, SOCK_STREAM, 0);
  setsockopt_err (listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on));

  bzero (&servaddr, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons (HTTP_SERVER_PORT);

  bind_err (listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
  listen_err (listenfd, BACKLOG);

  maxfd = listenfd;
  maxi = -1;
  for (i = 0; i < FD_SETSIZE; i++)
    client[i] = -1;

  FD_ZERO (&allset);
  FD_SET (listenfd, &allset);

  for (;;) {
    rset = allset;
    nready = select_err (maxfd + 1, &rset, NULL, NULL, NULL);

    if (FD_ISSET (listenfd, &rset)) {
      len = sizeof (peeraddr);
      peer = accept_err (listenfd, (struct sockaddr *) &peeraddr, &len);

      for (i = 0; i < FD_SETSIZE; i++)
        if (client[i] < 0) {
          client[i] = peer;
          break;
        }
      if (i == FD_SETSIZE) {
        fprintf (stderr, "too many clients");
        exit (EXIT_FAILURE);
      }

      FD_SET (peer, &allset);
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

      if (--nready <= 0)
        break;
    }
  }

  return 0;
}
