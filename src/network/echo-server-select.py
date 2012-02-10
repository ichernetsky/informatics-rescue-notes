import select

import aux

def main():
    args = aux.parse_args('Echo server with select-processing model.')
    server_socket = aux.listening_socket(args.port)

    socket_to_data = {}
    client_sockets = []
    all_sockets = [server_socket]

    while True:
        readable, _, _ = select.select(all_sockets, [], [], 0.05)

        if server_socket in readable:
            client_socket, _ = server_socket.accept()

            socket_to_data[client_socket] = ''
            client_sockets.append(client_socket)
            all_sockets.append(client_socket)

            readable.remove(server_socket)

        for client_socket in readable:
            data = client_socket.recv(1024)
            if not data:
                del socket_to_data[client_socket]
                client_sockets.remove(client_socket)
                all_sockets.remove(client_socket)
                client_socket.close()
            else:
                socket_to_data[client_socket] += data

        check_writability = [cs for cs in client_sockets
                             if socket_to_data[cs]]

        _, writable, _ = select.select([], check_writability, [], 0.05)

        for client_socket in writable:
            data = socket_to_data[client_socket]
            n = client_socket.send(data)
            if n > 0:
                socket_to_data[client_socket] = data[n:]

if __name__ == '__main__':
    main()
