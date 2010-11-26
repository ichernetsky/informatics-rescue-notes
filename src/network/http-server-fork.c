#include "netaux.h"

int main (void) {
  int listenfd, peer;
  pid_t child;

  set_signal_handler_err (SIGCHLD, sig_wait_child);
  listenfd = tcp_listen ();

  for (;;) {
    peer = tcp_accept (listenfd);

    child = fork_err ();
    if (child == 0) {
      close_err (listenfd);
      write_http_content (peer);
      return 0;
    }

    close_err (peer);
  }
}
