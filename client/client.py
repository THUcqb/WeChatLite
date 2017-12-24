import argparse
import socket
import info
from actions import ping, login, search, quit

parser = argparse.ArgumentParser()
parser.add_argument('--port', default=5000)
args = parser.parse_args()
HOST = '127.0.0.1'
PORT = int(args.port)

commands = {
    'ping': ping,
    'login': login,
    'search': search,
    'quit': quit,
}


def event_loop(s):
    while True:
        line = input()
        cmd = line.split(' ')[0]
        try:
            commands[cmd](line, s)
        except KeyError:
            print(info.CMD_NOT_EXIST)
        else:
            if cmd == 'quit': break


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    data = s.recv(1024).decode('utf-8')
    try:
        assert data[:14] == '<<JOIN, HELLO '
        print(info.CONNECT_OK)
    except AssertionError:
        print(info.CONNECT_FAIL)
    else:
        try:
            event_loop(s)
        except KeyboardInterrupt:
            quit("", s)
            print(info.DISCONNECT)
