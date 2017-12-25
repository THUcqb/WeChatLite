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
    'guest':
    {
        'ping': ping,
        'login': login,
        'quit': quit,
    },
    'user':
    {
        'ping': ping,
        'search': search,
        'quit': quit,
    }
}


def event_loop(s):
    logged_in = False
    while True:
        line = input().strip()
        cmd = line.split(' ')[0]
        mode = 'user' if logged_in else 'guest'
        try:
            logged_in = commands[mode][cmd](line, s)
        except KeyError:
            print(info.CMD_NOT_EXIST)
        else:
            if cmd == 'quit':
                break


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    try:
        s.connect((HOST, PORT))
        data = s.recv(1024).decode('utf-8')
        assert data[:5] == 'HELLO'
        print(info.CONNECT_OK)
    except AssertionError:
        print(info.CONNECT_FAIL)
    else:
        try:
            event_loop(s)
        except KeyboardInterrupt:
            quit("", s)
            print(info.DISCONNECT)
