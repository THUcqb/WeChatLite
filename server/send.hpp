#ifndef SEND_HPP
#define SEND_HPP

#include <unistd.h>
#include <iostream>
#include <string>
#include "typedef.hpp"

#define MAX_CLIENTS	100
client_t *clients[MAX_CLIENTS];

/* Send message to sender */
void send_to(std::string s, int connfd)
{
    write(connfd, s.c_str(), s.length());
}

#endif
