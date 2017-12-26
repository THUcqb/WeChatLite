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
        profiles[username]["friends"] = json::object();
        profiles[username]["msgbuffer"] = json::array();
        profiles[username]["filebuffer"] = json::array();
        profiles[username]["connfd"] = cli->connfd;
        cli->name = username;
        buff_out = R"({"status": "OK"})";
    }
    send_to(buff_out, cli->connfd);
}

void handle_search(client_t *cli)
{
    json users = json::object();
    for (auto iter = profiles.begin(); iter != profiles.end(); ++iter)
        users[iter.key()] = !iter.value()["connfd"].is_null();
    send_to(users.dump(), cli->connfd);
}

void handle_add(client_t *cli, json message)
{
    std::string target = message["friend"];
    std::string buff_out;
    if (!profiles[target].is_null())
    {
        profiles[cli->name]["friends"][target] = json::object();
        buff_out = R"({"status": "OK"})";
    }
    else
    {
        profiles.erase(target);
        buff_out = R"({"status": "ERROR"})";
    }
    send_to(buff_out, cli->connfd);
}

void handle_ls(client_t *cli)
{
    json friends = json::object();
    for (auto iter = profiles[cli->name]["friends"].begin();
         iter != profiles[cli->name]["friends"].end();
         ++iter)
        friends[iter.key()] = !profiles[iter.key()]["connfd"].is_null();
    send_to(friends.dump(), cli->connfd);
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

void handle_chat(client_t *cli, json message)
{
    std::string buff_out;
    std::string fri = message["friend"];
    if (profiles[cli->name]["friends"][fri].is_null())
    {
        profiles[cli->name]["friends"].erase(fri);
        buff_out = R"({"status": "ERROR"})";
    }
    else
    {
        profiles[cli->name]["inchat"] = fri;
        buff_out = R"({"status": "OK"})";
    }
    send_to(buff_out, cli->connfd);
}

void handle_exitchat(client_t *cli)
{
    std::string buff_out;
    if (!profiles[cli->name]["inchat"].is_null())
    {
        profiles[cli->name].erase("inchat");
        buff_out = R"({"status": "OK"})";
    }
    else
    {
        buff_out = R"({"status": "ERROR"})";
    }
    send_to(buff_out, cli->connfd);
}

void handle_sendmsg(client_t *cli, json message)
{
    message.erase("cmd");
    std::string fri = message["friend"];
    message["friend"] = cli->name;
    if (profiles[fri]["inchat"] == cli->name)
    {
        send_to(message.dump(), profiles[fri]["connfd"]);
    }
    else
    {
        profiles[fri]["msgbuffer"] += message;
    }
}

void handle_recvmsg(client_t *cli)
{
    json buff_out;
    buff_out["msg"] = profiles[cli->name]["msgbuffer"];
    profiles[cli->name]["msgbuffer"] = json::array();
    send_to(buff_out.dump(), cli->connfd);
}

void handle_sendfile(client_t *cli, json message)
{
    message.erase("cmd");
    std::string fri = message["friend"];
    message["friend"] = cli->name;
    profiles[fri]["filebuffer"] += message;
}

void handle_recvfile(client_t *cli)
{
    json buff_out;
    buff_out["files"] = profiles[cli->name]["filebuffer"];
    profiles[cli->name]["filebuffer"] = json::array();
    send_to(buff_out.dump(), cli->connfd);
}

#endif
