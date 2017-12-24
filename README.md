WeChatLite
==========

## Intro
Server initially borrowed from https://github.com/yorickdewid/Chat-Server

Simple chatroom in C. This project demonstrates the basic use of sockets.
You should specify the port, for example 5000.
The project is intended to run on Linux and Unix based systems.

## Build
### Server
First, fetch dependencies.
```bash
bash fetch.sh
```

Run GNU make in the repository
```bash
cd server
make
```

Then start
`./a.out 5000`

### Client
`python client.py --port 5000`

## Features (out of maintenance)
* Accept multiple client (max 100)
* Name and rename users
* Send private messages

## Chat commands (out of maintenance)

| Command       | Parameter             |                                     |
| ------------- | --------------------- | ----------------------------------- |
| \QUIT         |                       | Leave the chatroom                  |
| \PING         |                       | Test connection, responds with PONG |
| \NAME         | [nickname]            | Change nickname                     |
| \PRIVATE      | [reference] [message] | Send private message                |
| \ACTIVE       |                       | Show active clients                 |
| \HELP         |                       | Show this help                      |
