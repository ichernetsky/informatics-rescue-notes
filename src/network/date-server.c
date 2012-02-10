#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <error.h>
#include <errno.h>

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>

#include <time.h>

#define BUFFER_SIZE    1024
#define BACKLOG        5

int main(int argc, char *argv[])
{
    int listen_socket, client_socket;
    socklen_t socket_length;
    char *service;
    struct addrinfo hints, *addr, *addrs;
    struct sockaddr_storage client_addr;
    char buffer[BUFFER_SIZE + 1];     /* extra byte for '\0' */
    time_t ticks;
    int result;
    const int on = 1;

    if (argc != 2)
        error(1, 0, "usage: echo-client-basic <port>");

    service = argv[1];

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((result = getaddrinfo(NULL, service, &hints, &addrs)) != 0)
        error(1, 0, "getaddrinfo: %s", gai_strerror(result));
    addr = addrs;

    do {
        if ((listen_socket = socket(addr->ai_family, addr->ai_socktype,
                                    addr->ai_protocol)) < 0)
            continue;

        if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR,
                       &on, sizeof(on)) != 0)
            error(1, errno, "setsockopt");

        if (bind(listen_socket, addr->ai_addr, addr->ai_addrlen) == 0)
            break;

        if (close(listen_socket) != 0)
            error(1, errno, "close");
    } while ((addr = addr->ai_next) != NULL);

    if (addr == NULL)
        error(1, 0, "was not able to bind to specified network address and service");
    freeaddrinfo(addrs);

    if (listen(listen_socket, BACKLOG) != 0)
        error(1, errno, "listen");

    for (;;) {
        socket_length = sizeof(client_addr);
        if ((client_socket = accept(listen_socket,
                                    (struct sockaddr *) &client_socket,
                                    &socket_length)) < 0)
            error(1, errno, "accept");

        ticks = time(NULL);
        snprintf(buffer, sizeof(buffer), "%.24s\r\n", ctime(&ticks));
        if (write(client_socket, buffer, strlen(buffer)) < 0)
            error(1, errno, "write");
        /*
         *   ignoring the fact that write can write less bytes
         *   than specified
         */

        if (close(client_socket) < 0)
            error(1, errno, "close");
    }

    if (close(listen_socket) < 0)
        error(1, errno, "close");

    return 0;
}
