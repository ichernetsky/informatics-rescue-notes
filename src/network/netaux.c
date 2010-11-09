#include "netaux.h"

char HTTP_BUFFER[MAX_LINE];

void init_buffer (void) {
  int i;
  for (i = 0; i < MAX_LINE - 2; i++)
    HTTP_BUFFER[i] = 'a';
  HTTP_BUFFER[MAX_LINE - 2 - 1] = '\r';
  HTTP_BUFFER[MAX_LINE - 1 - 1] = '\n';
}

int set_signal_handler (int signo, sig_func func) {
  struct sigaction act, old_act;

  act.sa_handler = func;
  sigemptyset (&act.sa_mask);
  act.sa_flags = 0;
  if (signo != SIGALRM)
    act.sa_flags |= SA_RESTART;
  return sigaction (signo, &act, &old_act);
}

void sig_wait_child (int signo) {
  pid_t pid;
  int stat;
  while ((pid = waitpid (-1, &stat, WNOHANG)) > 0)
    ;
  return;
}

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
