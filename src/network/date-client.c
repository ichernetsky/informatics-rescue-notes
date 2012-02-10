#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <error.h>
#include <errno.h>

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUFFER_SIZE    1024

int main(int argc, char *argv[])
{
    char *server_host;
    char *service;
    int server_socket;
    struct addrinfo hints, *addr, *addrs;
    char buffer[BUFFER_SIZE + 1];     /* extra byte for '\0' */
    int n, result;

    if (argc != 3)
        error(1, 0, "usage: date-client <server-ip> <port>");

    server_host = argv[1];
    service = argv[2];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((result = getaddrinfo(server_host, service, &hints, &addrs)) != 0)
        error(1, 0, "getaddrinfo: %s", gai_strerror(result));
    addr = addrs;

    do {
        if ((server_socket = socket(addr->ai_family, addr->ai_socktype,
                                    addr->ai_protocol)) < 0)
            continue;

        if (connect(server_socket, addr->ai_addr, addr->ai_addrlen) == 0)
            break;

        if (close(server_socket) != 0)
            error(1, errno, "close");
    } while ((addr = addr->ai_next) != NULL);

    if (addr == NULL)
        error(1, 0, "was not able to connect to specified network address and service");
    freeaddrinfo(addrs);

    while ((n = read(server_socket, buffer, BUFFER_SIZE)) > 0) {
        buffer[n] = '\0';
        if (printf("%s", buffer) < 0)
            error(1, 0, "error of printing to stdout");
    }

    if (n < 0)
        error(1, errno, "read");

    if (close(server_socket) < 0)
        error(1, errno, "close");

    return 0;
}
