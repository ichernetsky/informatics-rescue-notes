import argparse
import socket
import sys

__all__ = ['parse_args', 'listening_socket', 'communicate']

def parse_args(description):
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('port', type=int, help='port to bind')
    return parser.parse_args()

def listening_socket(port):
    host = None
    family = socket.AF_UNSPEC
    socket_type = socket.SOCK_STREAM
    protocol = 0
    flags = socket.AI_PASSIVE

    server_socket = None
    for addrinfo in socket.getaddrinfo(host, port, family, socket_type,
                                       protocol, flags):
        family, socket_type, protocol, canonical_name, addr = addrinfo

        try:
            server_socket = socket.socket(family, socket_type, protocol)
        except socket.error, msg:
            server_socket = None
            continue

        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try:
            server_socket.bind(addr)
            server_socket.listen(1)
        except socket.error, msg:
            server_socket.close()
            server_socket = None
            continue

        break

    if server_socket is None:
        print 'could not open socket'
        sys.exit(1)

    return server_socket

def communicate(client_socket):
    while True:
        data = client_socket.recv(1024)
        if not data:
            break
        client_socket.sendall(data)
