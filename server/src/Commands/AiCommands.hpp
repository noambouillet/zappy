/*
** EPITECH PROJECT, 2026
** AI Commands
** File description:
** Definition of the commands used for the AI
*/

#ifndef AICOMMANDS_HPP_
#define AICOMMANDS_HPP_

#include <string>
#include <functional>

namespace ZappyServer {

static constexpr unsigned int AI_COMMANDS_NUMBER = 12;
class Server;
class Client;

struct AiCommandEntry {
    std::string_view name;
    std::function<void(Client &, Server &, const std::string &)> handler;
};

namespace AiCommands {
    void dispatch(Client &client, Server &server, const std::string &line);
    void forward(Client &client, Server &server, const std::string &args);
    void right(Client &client, Server &server, const std::string &args);
    void left(Client &client, Server &server, const std::string &args);
    void look(Client &client, Server &server, const std::string &args);
    void inventory(Client &client, Server &server, const std::string &args);
    void broadcast(Client &client, Server &server, const std::string &args);
    void connectNbr(Client &client, Server &server, const std::string &args);
    void fork(Client &client, Server &server, const std::string &args);
    void eject(Client &client, Server &server, const std::string &args);
    void take(Client &client, Server &server, const std::string &args);
    void set(Client &client, Server &server, const std::string &args);
    void incantation(Client &client, Server &server, const std::string &args);
}

}

#endif /* !AICOMMANDS_HPP_ */
