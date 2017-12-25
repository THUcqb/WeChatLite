import json
import info


def ping(line, s):
    s.send(json.dumps({'cmd': 'ping'}).encode('utf-8'))
    data = s.recv(1024).decode('utf-8')
    print(data)


def search(line, s):
    s.send(json.dumps({'cmd': 'search'}).encode('utf-8'))
    data = json.loads(s.recv(1024).decode('utf-8'))
    print('All Users:')
    for user in data:
        print(user)


def quit(line, s):
    s.send(json.dumps({'cmd': 'quit'}).encode('utf-8'))
    data = s.recv(1024).decode('utf-8')
    print(repr(data))


def login(line, s):
    try:
        data = {}
        [data['cmd'], data['username'], data['password']] = line.split(' ')
        s.send(json.dumps(data).encode('utf-8'))
        data = json.loads(s.recv(1024).decode('utf-8'))
        if data['status'] == 'OK':
            print(info.LOGIN_OK)
        else:
            print(info.LOGIN_ERROR)
        return True
    except ValueError:
        print(info.LOGIN_ARG_ERROR)
