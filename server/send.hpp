#ifndef SEND_HPP
#define SEND_HPP

#include <unistd.h>
#include <iostream>
#include <string>
#include "typedef.hpp"

#define MAX_CLIENTS	100
client_t *clients[MAX_CLIENTS];

/* Send message to all clients */
void send_broadcast(std::string s)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i])
        {
            write(clients[i]->connfd, s.c_str(), s.length());
        }
    }
}

/* Send message to sender */
void send_self(std::string s, int connfd)
{
    write(connfd, s.c_str(), s.length());
}

/* Send message to client */
void send_to_peer(std::string s, int uid)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i] && clients[i]->uid == uid)
        {
            write(clients[i]->connfd, s.c_str(), s.length());
        }
    }
}

#endif
