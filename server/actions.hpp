#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include <iostream>
#include <string>
#include "typedef.hpp"
#include "json.hpp"
#include "send.hpp"

using json = nlohmann::json;
/* User profile indexed by username */
json profiles;

void handle_login(client_t *cli, json message)
{
    std::string username = message["username"], password = message["password"];
    std::string buff_out;
    if (!profiles[username].is_null())
    {
        // Sign in
        if (profiles[username]["password"] == password &&
            profiles[username]["connfd"].is_null())
        {
            profiles[username]["connfd"] = cli->connfd;
            cli->name = username;
            buff_out = R"({"status": "OK"})";
        }
        else
        {
            buff_out = R"({"status": "ERROR"})";
        }
    }
    else
    {
        // Sign up
        profiles[username]["password"] = password;
        profiles[username]["connfd"] = cli->connfd;
        cli->name = username;
        buff_out = R"({"status": "OK"})";
    }
    send_to(buff_out, cli->connfd);
}

void handle_search(client_t *cli)
{
    auto users = json::array();
    for (auto iter = profiles.begin(); iter != profiles.end(); ++iter)
        users += iter.key();
    send_to(users.dump(), cli->connfd);
}

void handle_quit(client_t *cli)
{
    /* Erase online info */
    if (!(cli->name).empty())
    {
        profiles[cli->name].erase("connfd");
    }
    auto buff_out = "BYE " + cli->name;
    send_to(buff_out, cli->connfd);
}

#endif
