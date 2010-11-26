#include "netaux.h"

static void *serve (void *arg);

int main (void) {
  int listenfd, peer;
  pthread_t thread_id;

  listenfd = tcp_listen ();

  for (;;) {
    peer = tcp_accept (listenfd);
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
