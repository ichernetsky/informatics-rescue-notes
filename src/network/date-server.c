#include <time.h>
#include "netaux.h"

int main (int argc, char *argv[]) {
  int listenfd, peer, ret, len;
  char *service = "13";
  const int on = 1;
  char line[MAX_LINE + 1];
  struct addrinfo hints, *res, *ressave;
  struct sockaddr_storage peeraddr;
  time_t ticks;

  if (argc != 1 && argc != 2) {
    fprintf (stderr, "usage: %s [<service-or-port>]\n", argv[0]);
    exit (1);
  }

  if (argc == 2)
    service = argv[1];

  bzero (&hints, sizeof (hints));
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if ((ret = getaddrinfo (NULL, service, &hints, &res)) != 0)
    err_quit (gai_strerror (ret));
  ressave = res;

  do {
    if ((listenfd = socket (res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
      continue;
    if (setsockopt (listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on)) != 0)
      err_quit ("setsockopt");
    if (bind (listenfd, res->ai_addr, res->ai_addrlen) == 0)
      break;

    if (close (listenfd) != 0)
      err_quit ("close");
  } while ((res = res->ai_next) != NULL);

  if (res == NULL)
    err_quit (NULL);
  freeaddrinfo (ressave);

  if (listen (listenfd, BACKLOG) != 0)
    err_quit ("listen");

  for (;;) {
    len = sizeof (peeraddr);
    if ((peer = accept (listenfd, (struct sockaddr *) &peeraddr, &len)) < 0)
      err_quit ("accept");

    ticks = time (NULL);
    snprintf (line, sizeof (line), "%.24s\r\n", ctime (&ticks));
    if (write (peer, line, strlen (line)) < 0)
      err_quit ("write");

    if (close (peer) != 0)
      err_quit ("close");
  }

  return 0;
}
