import select

import aux

def main():
    args = aux.parse_args('Echo server with poll-processing model.')
    server_socket = aux.listening_socket(args.port)
    server_fileno = server_socket.fileno()

    fileno_to_data = {}
    fileno_to_socket = {}

    read_poll = select.poll()
    read_poll.register(server_fileno, select.POLLIN)

    while True:
        for fileno, eventmask in read_poll.poll(50):
            if fileno == server_fileno:
                client_socket, _ = server_socket.accept()
                client_fileno = client_socket.fileno()

                fileno_to_data[client_fileno] = ''
                fileno_to_socket[client_fileno] = client_socket

                read_poll.register(client_fileno, select.POLLIN)
            else:
                client_socket = fileno_to_socket[fileno]

                data = client_socket.recv(1024)
                if not data:
                    read_poll.unregister(fileno)
                    del fileno_to_data[fileno]
                    del fileno_to_socket[fileno]
                    client_socket.close()
                else:
                    fileno_to_data[fileno] += data

        check_writability = [f for f, d in fileno_to_data.iteritems() if d]
        if not check_writability:
            continue

        write_poll = select.poll()
        for fileno in check_writability:
            write_poll.register(fileno, select.POLLOUT)

        for fileno, eventmask in write_poll.poll(50):
            if eventmask & (select.POLLERR | select.POLLHUP):
                read_poll.unregister(fileno)
                fileno_to_socket[fileno].close()
                del fileno_to_data[fileno]
                del fileno_to_socket[fileno]
                continue

            client_socket = fileno_to_socket[fileno]
            data = fileno_to_data[fileno]

            n = client_socket.send(data)
            if n > 0:
                fileno_to_data[fileno] = data[n:]

if __name__ == '__main__':
    main()
