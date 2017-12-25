import json
import info


def ping(s, line):
    s.send(json.dumps({'cmd': 'ping'}).encode('utf-8'))
    data = s.recv(1024).decode('utf-8')
    print(data)


def search(s, line):
    s.send(json.dumps({'cmd': 'search'}).encode('utf-8'))
    data = json.loads(s.recv(1024).decode('utf-8'))
    print('All Users:')
    for user in data:
        print(user)


def quit(s, line=""):
    s.send(json.dumps({'cmd': 'quit'}).encode('utf-8'))
    data = s.recv(1024).decode('utf-8')
    print(repr(data))


def login(s, line):
    try:
        data = {}
        data['cmd'], data['username'], data['password'] = line.split(' ')
        s.send(json.dumps(data).encode('utf-8'))
        data = json.loads(s.recv(1024).decode('utf-8'))
        if data['status'] == 'OK':
            print(info.LOGIN_OK)
            return True
        else:
            print(info.LOGIN_ERROR)
    except ValueError:
        print(info.LOGIN_ARG_ERROR)


def add(s, line):
    try:
        data = {}
        data['cmd'], data['friend'] = line.split(' ')
        s.send(json.dumps(data).encode('utf-8'))
        data = json.loads(s.recv(1024).decode('utf-8'))
        if data['status'] == 'OK':
            print(info.ADD_OK)
        else:
            print(info.ADD_ERROR)
    except ValueError:
        print(info.ADD_ARG_ERROR)


def ls(s, line):
    s.send(json.dumps({'cmd': 'ls'}).encode('utf-8'))
    data = json.loads(s.recv(1024).decode('utf-8'))
    print('Your friends:')
    for user in data:
        print(user)
