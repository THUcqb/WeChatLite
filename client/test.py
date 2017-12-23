import argparse
import socket

parser = argparse.ArgumentParser()
parser.add_argument('--port', default=5000)

args = parser.parse_args()


def test(host, port):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, port))
        data = s.recv(1024).decode('utf-8')
        print(repr(data))
        assert data[:14] == '<<JOIN, HELLO '

        s.send(b'{"cmd": "ping"}')
        data = s.recv(1024).decode('utf-8')
        print(repr(data))
        assert data == '<<PONG'

        s.send(b'{"cmd": "login", "username": "cqb", "password": "howareu"}')
        data = s.recv(1024).decode('utf-8')
        print(repr(data))
        assert data[:10] == '<<RENAME, '

        s.send(b'{"cmd": "search"}')
        data = s.recv(1024).decode('utf-8')
        print(repr(data))
        assert data[:9] == '<<CLIENT '

        s.send(b'{"cmd": "quit"}')
        data = s.recv(1024).decode('utf-8')
        print(repr(data))
        assert data[:12] == '<<LEAVE, BYE'


HOST = '127.0.0.1'
PORT = int(args.port)

test(HOST, PORT)
