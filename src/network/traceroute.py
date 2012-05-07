import argparse
import socket

icmp = socket.getprotobyname('icmp')
udp = socket.getprotobyname('udp')

def create_sockets(ttl):
    recv_socket = socket.socket(socket.AF_INET, socket.SOCK_RAW, icmp)
    send_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, udp)

    send_socket.setsockopt(socket.SOL_IP, socket.IP_TTL, ttl)
    return recv_socket, send_socket

def main(host, port, hops):
    addr = socket.gethostbyname(host)

    ttl = 1
    while True:
        recv_socket, send_socket = create_sockets(ttl)
        recv_socket.bind(("", port))
        send_socket.sendto("", (host, port))

        curr_addr = None
        curr_name = None
        try:
            _, curr_addr = recv_socket.recvfrom(512)
            curr_addr = curr_addr[0]
            try:
                curr_name = socket.gethostbyaddr(curr_addr)[0]
            except socket.error:
                curr_name = curr_addr
        except socket.error:
            pass
        finally:
            send_socket.close()
            recv_socket.close()

        if curr_addr is not None:
            curr_host = "%s (%s)" % (curr_name, curr_addr)
        else:
            curr_host = "*"
        print "%d\t%s" % (ttl, curr_host)

        ttl += 1
        if curr_addr == addr or ttl > hops:
            break

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='traces path to a network host')
    parser.add_argument('host', metavar='host', type=unicode,
                        help='host which is to trace path to')
    parser.add_argument('-p', '--port', dest='port', metavar='port',
                        help='port to use for connection [default: %(default)d]',
                        default=45678, type=int)
    parser.add_argument('-m', '--max-hops', dest='hops', metavar='hops',
                        help='max hops [default: %(default)d]',
                        default=30, type=int)

    args = parser.parse_args()
    main(args.host, args.port, args.hops)
