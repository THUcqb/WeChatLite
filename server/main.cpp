#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <errno.h>
#include <cstring>
#include <pthread.h>
#include <iostream>
#include <string>
#include "typedef.hpp"
#include "actions.hpp"
#include "json.hpp"

using json = nlohmann::json;

static unsigned int cli_count = 0;
static int uid = 0;


/* Add client to queue */
void queue_add(client_t *cl)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (!clients[i])
        {
            clients[i] = cl;
            return;
        }
    }
}

/* Delete client from queue */
void queue_delete(int uid)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i] && clients[i]->uid == uid)
        {
            clients[i] = NULL;
            return;
        }
    }
}

/* Print ip address */
void print_client_addr(struct sockaddr_in addr)
{
    printf("%d.%d.%d.%d",
    addr.sin_addr.s_addr & 0xFF,
    (addr.sin_addr.s_addr & 0xFF00)>>8,
    (addr.sin_addr.s_addr & 0xFF0000)>>16,
    (addr.sin_addr.s_addr & 0xFF000000)>>24);
}

/* Handle all communication with the client */
void *handle_client(void *arg)
{
    std::string buff_out;
    char buff_in[1024];
    int rlen;

    cli_count++;
    client_t *cli = (client_t *)arg;

    printf("Accept ");
    print_client_addr(cli->addr);
    printf(" referenced by %d\n", cli->uid);

    buff_out = "HELLO" + cli->name;
    send_to(buff_out, cli->connfd);

    /* Receive input from client */
    while ((rlen = read(cli->connfd, buff_in, sizeof(buff_in)-1)) > 0)
    {
        buff_in[rlen] = '\0';
        /* Ignore empty buffer */
        if (!strlen(buff_in))
        {
            continue;
        }

        std::cerr << "Log, received raw message \'" << buff_in
                  << "\' from " << cli->uid << std::endl;

        auto message = json::parse(buff_in);
        auto cmd = message["cmd"];

        if (cmd == "ping")
        {
            send_to("<<PONG", cli->connfd);
        }
        else if (cmd == "login")
        {
            handle_login(cli, message);
        }
        else if (cmd == "search")
        {
            handle_search(cli);
        }
        else if (cmd == "add")
        {
            handle_add(cli, message);
        }
        else if (cmd == "ls")
        {
            handle_ls(cli);
        }
        else if (cmd == "quit")
        {
            handle_quit(cli);
            break;
        }
        else
        {
            std::cerr << "Log, ignoring invalid msg above" << std::endl;
        }
    }

    /* Close connection */
    close(cli->connfd);

    /* Delete client from queue and yeild thread */
    queue_delete(cli->uid);
    printf("Leave ");
    print_client_addr(cli->addr);
    printf(" referenced by %d\n", cli->uid);
    delete cli;
    cli_count--;
    pthread_detach(pthread_self());

    return NULL;
}

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    pthread_t tid;

    /* Socket settings */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    /* Ignore pipe signals */
    signal(SIGPIPE, SIG_IGN);

    /* Bind */
    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Socket binding failed");
        return 1;
    }

    /* Listen */
    if (listen(listenfd, 10) < 0)
    {
        perror("Socket listening failed");
        return 1;
    }

    std::cerr << "<[SERVER STARTED]>" << std::endl;

    /* Accept clients */
    while (true)
    {
        socklen_t client = sizeof(cli_addr);
        connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &client);

        /* Check if max clients is reached */
        if ((cli_count + 1) == MAX_CLIENTS)
        {
            std::cerr << "<<MAX CLIENTS REACHED\n";
            std::cerr << "<<REJECT ";
            print_client_addr(cli_addr);
            std::cerr << "\n";
            close(connfd);
            continue;
        }

        /* Client settings */
        client_t *cli = new client_t;
        cli->addr = cli_addr;
        cli->connfd = connfd;
        cli->uid = uid++;

        /* Add client to the queue and fork thread */
        queue_add(cli);
        pthread_create(&tid, NULL, &handle_client, (void*)cli);
    }
}
