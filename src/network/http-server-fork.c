#include "netaux.h"

int main (int argc, char *argv[]) {
  int listenfd, peer, len;
  struct sockaddr_in servaddr, peeraddr;
  int on = 1;
  pid_t child;

  listenfd = socket_err (AF_INET, SOCK_STREAM, 0);
  setsockopt_err (listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on));

  bzero (&servaddr, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons (HTTP_SERVER_PORT);

  set_signal_handler_err (SIGCHLD, sig_wait_child);

  bind_err (listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
  listen_err (listenfd, BACKLOG);

  for (;;) {
    len = sizeof (peeraddr);
    if ((peer = accept_err (listenfd, (struct sockaddr *) &peeraddr, &len)) < 0) {
      if (errno == EINTR || errno == ECONNABORTED)
        continue;
    }

    child = fork_err ();
    if (child == 0) {
      close_err (listenfd);

      write_http_content (peer);

      return 0;
    }

    close_err (peer);
  }
}
