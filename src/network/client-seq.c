#include "netaux.h"

int main (int argc, char *argv[]) {
  int peer, n;
  uint16_t port = 0;
  char line[MAX_LINE + 1];
  struct sockaddr_in peer_addr;

  if (argc != 3) {
    fprintf (stderr, "usage: %s <address> <port>\n", argv[0]);
    exit (1);
  }
  port = (uint16_t) atoi (argv[2]);

  if ((peer = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
    perror (NULL);
    exit (1);
  }

  bzero (&peer_addr, sizeof (peer_addr));
  peer_addr.sin_family = AF_INET;
  peer_addr.sin_port = htons (port);
  if (inet_pton (AF_INET, argv[1], &peer_addr.sin_addr) <= 0) {
    perror (NULL);
    exit (1);
  }

  if (connect (peer, (struct sockaddr *) &peer_addr, sizeof (peer_addr)) < 0) {
    perror (NULL);
    exit (1);
  }

  snprintf (line, MAX_LINE + 1, "GET / HTTP/1.1\r\n");
  if ((n = write_line (peer, line, strlen (line))) < 0) {
    perror (NULL);
    exit (1);
  }

  snprintf (line, MAX_LINE + 1, "Host: %s\r\n", argv[1]);
  if ((n = write_line (peer, line, strlen (line))) < 0) {
    perror (NULL);
    exit (1);
  }

  snprintf (line, MAX_LINE + 1, "\r\n");
  if ((n = write_line (peer, line, strlen (line))) < 0) {
    perror (NULL);
    exit (1);
  }

  printf ("asdf\n");
  fflush (stderr);
  while ((n = read (peer, line, MAX_LINE)) > 0) {
    line[n] = 0;
    if (printf ("%s", line) < 0) {
      perror (NULL);
      exit (1);
    }
  }
  if (n < 0) {
    perror (NULL);
    exit (1);
  }

  return EXIT_SUCCESS;
}
