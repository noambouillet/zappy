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
#include <string_view>

namespace ZappyServer {

static constexpr unsigned int AI_COMMANDS_NUMBER = 12;
class Server;
class Client;

enum class ResourceType {
    FOOD = 0,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME,
    MAX
};

enum class Direction { 
    NONE = 0,
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4,
    MAX = 5
};

struct Vector2D {
    int x;
    int y;
};

using AiCommandHandler = std::function<void(Client &, Server &, const std::string &)>;

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
