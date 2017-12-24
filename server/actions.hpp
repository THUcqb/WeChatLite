#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include "typedef.hpp"
#include "json.hpp"
#include "send.hpp"

using json = nlohmann::json;

void handle_login(client_t *cli, json message)
{
    auto username = message["username"], password = message["password"];
    if (!username.is_null() && !password.is_null())
    {
        auto buff_out = "<<RENAME, " + cli->name + " TO " + username.dump();
        cli->name = username;
        send_broadcast(buff_out);
    }
    else
    {
        send_self("<<NAME CANNOT BE NULL", cli->connfd);
    }
}

void handle_search(client_t *cli)
{
    send_self("SEARCH NOT IMPLEMENTED", cli->connfd);
}

#endif
