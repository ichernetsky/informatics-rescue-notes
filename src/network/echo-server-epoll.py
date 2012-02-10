import select

import aux

def main():
    args = aux.parse_args('Echo server with epoll-processing model.')
    server_socket = aux.listening_socket(args.port)
    server_fileno = server_socket.fileno()

    fileno_to_data = {}
    fileno_to_socket = {}

    read_epoll = select.epoll()
    read_epoll.register(server_fileno, select.EPOLLIN)

    while True:
        for fileno, eventmask in read_epoll.poll(0.05):
            if fileno == server_fileno:
                client_socket, _ = server_socket.accept()
                client_fileno = client_socket.fileno()

                fileno_to_data[client_fileno] = ''
                fileno_to_socket[client_fileno] = client_socket

                read_epoll.register(client_fileno, select.EPOLLIN)
            else:
                client_socket = fileno_to_socket[fileno]

                data = client_socket.recv(1024)
                if not data:
                    read_epoll.unregister(fileno)
                    del fileno_to_data[fileno]
                    del fileno_to_socket[fileno]
                    client_socket.close()
                else:
                    fileno_to_data[fileno] += data

        check_writability = [f for f, d in fileno_to_data.iteritems() if d]
        if not check_writability:
            continue

        write_epoll = select.epoll()
        for fileno in check_writability:
            write_epoll.register(fileno, select.EPOLLOUT)

        for fileno, eventmask in write_epoll.poll(0.05):
            if eventmask & (select.EPOLLERR | select.EPOLLHUP):
                read_epoll.unregister(fileno)
                fileno_to_socket[fileno].close()
                del fileno_to_data[fileno]
                del fileno_to_socket[fileno]
                continue

            client_socket = fileno_to_socket[fileno]
            data = fileno_to_data[fileno]

            n = client_socket.send(data)
            if n > 0:
                fileno_to_data[fileno] = data[n:]

        write_epoll.close()

if __name__ == '__main__':
    main()
