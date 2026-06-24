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
#include <array>

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

static constexpr std::array<std::array<unsigned int, 7>, 7> INCANTATION_REQUIREMENTS = {{
    {{1, 1, 0, 0, 0, 0, 0}},
    {{2, 1, 1, 1, 0, 0, 0}},
    {{2, 2, 0, 1, 0, 2, 0}},
    {{4, 1, 1, 2, 0, 1, 0}},
    {{4, 1, 2, 1, 3, 0, 0}},
    {{6, 1, 2, 3, 0, 1, 0}},
    {{6, 2, 2, 2, 2, 2, 1}}
}};

using AiCommandHandler = std::function<void(Client &, Server &, const std::string &)>;

enum class CommandStartResult {
    RUNNING,
    CONSUMED,
    FAILED
};

namespace AiCommands {
    unsigned int getCommandTicks(const std::string &commandName);
    CommandStartResult begin(Client &client, Server &server, const std::string &line);
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
