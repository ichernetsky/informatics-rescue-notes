#include "netaux.h"

static void *serve (void *arg);

int main (void) {
  int listenfd, peer;
  socklen_t len;
  struct sockaddr_in servaddr, peeraddr;
  int on = 1;
  pthread_t thread_id;

  listenfd = socket_err (AF_INET, SOCK_STREAM, 0);
  setsockopt_err (listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on));

  bzero (&servaddr, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons (HTTP_SERVER_PORT);

  bind_err (listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
  listen_err (listenfd, BACKLOG);

  for (;;) {
    len = sizeof (peeraddr);
    if ((peer = accept_err (listenfd, (struct sockaddr *) &peeraddr, &len)) < 0) {
      if (errno == EINTR || errno == ECONNABORTED)
        continue;
    }

    pthread_create_err (&thread_id, NULL, &serve, (void *) (long int) peer);
  }

  return 0;
}

static void *serve (void *arg) {
  int peer = (int) (long int) arg;

  pthread_detach_err (pthread_self ());
  write_http_content (peer);
  close_err (peer);

  return NULL;
}
