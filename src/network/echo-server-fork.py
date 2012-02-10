import os
import signal

import aux

def main():
    args = aux.parse_args('Echo server with fork processing model.')
    server_socket = aux.listening_socket(args.port)

    signal.signal(signal.SIGCHLD, signal.SIG_IGN)

    while True:
        client_socket, client_addr = server_socket.accept()

        if os.fork() == 0:
            server_socket.close()
            aux.communicate(client_socket)
            client_socket.close()
            return
        else:
            client_socket.close()

if __name__ == '__main__':
    main()
