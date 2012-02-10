#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <error.h>
#include <errno.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE    1024

int main(int argc, char *argv[])
{
    char *server_ip;
    int server_port;
    int server_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE + 1];     /* extra byte for '\0' */
    int n, result;

    if (argc != 3)
        error(1, 0, "usage: date-client-basic <server-ip> <port>");

    server_ip = argv[1];
    server_port = atoi(argv[2]);
    if (server_port <= 0)
        error(1, 0, "port number must be a number > 0");

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        (error(1, errno, "socket"));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    if ((result = inet_pton(AF_INET, server_ip, &server_addr.sin_addr)) < 0)
        error(1, errno, "inet_pton");
    else if (result == 0)
        error(1, 0, "IPv4 address must be specified");

    if (connect(server_socket, (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0)
        error(1, errno, "connect");

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
