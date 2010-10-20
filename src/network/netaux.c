#include "netaux.h"

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
