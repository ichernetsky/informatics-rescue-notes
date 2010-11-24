#include "netaux.h"

int main (void) {
  unsigned int i, maxi;
  int listenfd, peer;
  int nready;
  struct sockaddr_in servaddr, peeraddr;
  socklen_t len;
  int on = 1;
  struct pollfd *client;
  struct rlimit rl;
  rlim_t openmax = 1024;

  if (getrlimit (RLIMIT_NOFILE, &rl) != -1)
    openmax = rl.rlim_max;
  client = (struct pollfd *) malloc (sizeof (struct pollfd) * openmax);

  listenfd = socket_err (AF_INET, SOCK_STREAM, 0);
  setsockopt_err (listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on));

  bzero (&servaddr, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
  servaddr.sin_port = htons (HTTP_SERVER_PORT);

  bind_err (listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
  listen_err (listenfd, BACKLOG);

  client[0].fd = listenfd;
  client[0].events = POLLIN;
  for (i = 1; i < openmax; i++)
    client[i].fd = -1;
  maxi = 0;

  for (;;) {
    nready = poll_err (client, maxi + 1, -1);

    if (client[0].revents & POLLIN) {
      len = sizeof (peeraddr);
      peer = accept_err (listenfd, (struct sockaddr *) &peeraddr, &len);

      for (i = 1; i < openmax; i++)
        if (client[i].fd < 0) {
          client[i].fd = peer;
          break;
        }

      if (i == openmax) {
        fprintf (stderr, "too many clients");
        exit (EXIT_FAILURE);
      }

      client[i].events = POLLOUT;
      if (i > maxi)
        maxi = i;

      if (--nready <= 0)
        continue;
    }

    for (i = 1; i <= maxi; i++) {
      if ((peer = client[i].fd) < 0)
        continue;

      if (client[i].revents & POLLOUT) {
        write_http_content (peer);
        close_err (peer);
        client[i].fd = -1;

        if (--nready <=0)
          break;
      }
    }
  }

  return 0;
}
