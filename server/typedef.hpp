#ifndef TYPEDEF_HPP
#define TYPEDEF_HPP

#include <netinet/in.h>
#include <iostream>
#include <string>

/* Client structure */
typedef struct
{
    struct sockaddr_in addr;	/* Client remote address */
    int connfd;			/* Connection file descriptor */
    int uid;			/* Client unique identifier */
    std::string name;			/* Client name */
} client_t;

#endif
