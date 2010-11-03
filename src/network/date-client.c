#include "netaux.h"

int main (int argc, char *argv[]) {
  int peer, n, ret;
  char *host;
  char *service = "13";
  char line[MAX_LINE + 1];
  struct addrinfo hints, *res, *ressave;

  if (argc != 2 && argc != 3) {
    fprintf (stderr, "usage: %s <name-or-address>[ <port>]\n", argv[0]);
    exit (1);
  }

  host = argv[1];
  if (argc == 3)
    service = argv[2];

  bzero (&hints, sizeof (hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if ((ret = getaddrinfo(host, service, &hints, &res)) != 0)
    err_quit (gai_strerror (ret));
  ressave = res;

  do {
    if ((peer = socket (res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
      continue;
    if (connect (peer, res->ai_addr, res->ai_addrlen) == 0)
      break;

    if (close (peer) != 0)
      err_quit ("close");
  } while ((res = res->ai_next) != NULL);

  if (res == NULL)
    err_quit (NULL);
  freeaddrinfo (ressave);

  while ((n = read (peer, line, MAX_LINE)) > 0) {
    line[n] = 0;
    if (printf ("%s", line) < 0)
      err_quit ("printf");
  }
  if (n < 0)
    err_quit ("read");

  return EXIT_SUCCESS;
}
