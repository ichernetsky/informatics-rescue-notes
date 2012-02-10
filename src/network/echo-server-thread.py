import threading

import aux

class Worker(threading.Thread):
    def __init__(self, client_socket):
        super(Worker, self).__init__()

        self.client_socket = client_socket
        self.daemon = True

    def run(self):
        aux.communicate(self.client_socket)
        self.client_socket.close()

def main():
    args = aux.parse_args('Echo server with threaded-processing model.')
    server_socket = aux.listening_socket(args.port)

    while True:
        client_socket, client_addr = server_socket.accept()

        worker = Worker(client_socket)
        worker.start()

if __name__ == '__main__':
    main()
