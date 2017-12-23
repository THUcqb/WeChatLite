WeChatLite
==========

## Intro
Server initially borrowed from https://github.com/yorickdewid/Chat-Server

Simple chatroom in C. This project demonstrates the basic use of sockets.
You should specify the port, for example 5000.
The project is intended to run on Linux and Unix based systems.

## Build
### Server
Run GNU make in the repository
`make`

Then start
`./chat_server 5000`

### Client
`python test.py --port 5000`

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
