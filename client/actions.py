import json
import info


def ping(s, line):
    s.send(json.dumps({'cmd': 'ping'}).encode('utf-8'))
    data = s.recv(1024).decode('utf-8')
    print(data)


def search(s, line):
    s.send(json.dumps({'cmd': 'search'}).encode('utf-8'))
    data = json.loads(s.recv(1024).decode('utf-8'))
    print(info.SEARCH_USERS)
    for user in data:
        print(user)


def quit(s, line=""):
    s.send(json.dumps({'cmd': 'quit'}).encode('utf-8'))
    data = s.recv(1024).decode('utf-8')
    print(data)


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
    print(info.LS_FRIENDS)
    for user in data:
        print(user)


def recvmsg(s, line):
    s.send(json.dumps({'cmd': 'recvmsg'}).encode('utf-8'))
    data = json.loads(s.recv(1024).decode('utf-8'))
    print(info.RECVMSG)
    for item in data['msg']:
        print('From %s: %s' % (item['friend'], item['msg']))


def chat(s, line):
    try:
        data = {}
        data['cmd'], data['friend'] = line.split(' ')
        s.send(json.dumps(data).encode('utf-8'))
        response = json.loads(s.recv(1024).decode('utf-8'))
        if response['status'] == 'OK':
            print(info.CHAT_OK)
            while True:
                line = input().strip()
                cmd = line.split(' ')[0]
                if cmd == 'exit':
                    break
                else:
                    try:
                        commands['chat'][cmd](s, data['friend'], line)
                    except KeyError:
                        print(info.CMD_NOT_EXIST)
            print(info.CHAT_END)
        else:
            print(data['friend'] + info.CHAT_ERROR)
    except ValueError:
        print(info.CHAT_ARG_ERROR)


def sendmsg(s, friend, line):
    data = {}
    data['cmd'], data['msg'] = line.split(' ')
    data['friend'] = friend
    s.send(json.dumps(data).encode('utf-8'))


def sendfile(s, friend, line):
    data['cmd'], data['filename'] = line.split(' ')
    data['friend'] = friend
    print("Send file not implemented.")


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
        'add': add,
        'ls': ls,
        'recvmsg': recvmsg,
        'chat': chat,
        'quit': quit,
    },
    'chat':
    {
        'sendmsg': sendmsg,
        'sendfile': sendfile,
    }
}
