#include "netaux.h"

int main (int argc, char *argv[]) {
  int listenfd, peer, len;
  struct sockaddr_in servaddr, peeraddr;
  char line[MAX_LINE + 1];
  int on = 1;
  pid_t child;

  if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    err_quit ("socket");
  if (setsockopt (listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on)) != 0)
      err_quit ("setsockopt");

  bzero (&servaddr, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons (HTTP_SERVER_PORT);

  if (bind (listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr)) < 0)
    err_quit ("bind");

  if (listen (listenfd, BACKLOG) != 0)
    err_quit ("listen");

  for (;;) {
    len = sizeof (peeraddr);
    if ((peer = accept (listenfd, (struct sockaddr *) &peeraddr, &len)) < 0)
      err_quit ("accept");

    if ((child = fork ()) < 0)
      err_quit ("fork");
    if (child == 0) {
      if (close (listenfd) != 0)
        err_quit ("close");

      snprintf (line, sizeof (line), "Wow!\r\n");
      if (write (peer, line, strlen (line)) < 0)
        err_quit ("write");

      return 0;
    }

    if (close (peer) != 0)
      err_quit ("close");
  }
}
