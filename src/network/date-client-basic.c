#include "netaux.h"

int main (int argc, char *argv[]) {
  int peer, n, ret;
  uint16_t port = 13;
  char line[MAX_LINE + 1];
  struct sockaddr_in peer_addr;

  if (argc != 2 && argc != 3) {
    fprintf (stderr, "usage: %s <address>[ <port>]\n", argv[0]);
    exit (1);
  }

  if (argc == 3)
    port = (uint16_t) atoi (argv[2]);

  if ((peer = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    err_quit ("socket");

  bzero (&peer_addr, sizeof (peer_addr));
  peer_addr.sin_family = AF_INET;
  peer_addr.sin_port = htons (port);
  if ((ret = inet_pton (AF_INET, argv[1], &peer_addr.sin_addr)) < 0)
    err_quit ("inet_pton");
  else if (ret == 0) {
    fprintf (stderr, "IPv4 address must be specified\n");
    exit (1);
  }

  if (connect (peer, (struct sockaddr *) &peer_addr, sizeof (peer_addr)) < 0)
    err_quit ("connect");

  while ((n = read (peer, line, MAX_LINE)) > 0) {
    line[n] = 0;
    if (printf ("%s", line) < 0)
      err_quit ("printf");
  }
  if (n < 0)
    err_quit ("read");

  return EXIT_SUCCESS;
}
