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
        s.send(b'\\PING')
        data = s.recv(1024).decode('utf-8')
        print(repr(data))
        assert data == '<<PONG'
        s.send(b'\\NAME cqb')
        data = s.recv(1024).decode('utf-8')
        print(repr(data))
        assert data[:10] == '<<RENAME, '
        s.send(b'\\ACTIVE')
        data = s.recv(1024).decode('utf-8')
        print(data)
        s.send(b'\\QUIT')
        data = s.recv(1024).decode('utf-8')
        print(data)



HOST = 'localhost'
PORT = int(args.port)

test(HOST, PORT)
