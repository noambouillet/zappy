/*
** EPITECH PROJECT, 2026
** AI Commands
** File description:
** Implementation of the commands used for the AI
*/

#include "AiCommands.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include <array>
#include <string_view>

namespace ZappyServer {

static const std::array<AiCommandEntry, AI_COMMANDS_NUMBER> aiDispatch = {{
    { "Forward", AiCommands::forward },
    { "Right", AiCommands::right },
    { "Left", AiCommands::left },
    { "Look", AiCommands::look },
    { "Inventory", AiCommands::inventory },
    { "Connect_nbr", AiCommands::connectNbr },
    { "Fork", AiCommands::fork },
    { "Eject", AiCommands::eject },
    { "Incantation", AiCommands::incantation },
    { "Broadcast", AiCommands::broadcast },
    { "Take", AiCommands::take },
    { "Set", AiCommands::set },
}};

void AiCommands::dispatch(Client &client, Server &server, const std::string &line)
{
    std::size_t spacePos = line.find(' ');
    std::string cmd = line.substr(0, spacePos);
    std::string args = spacePos != std::string::npos ? line.substr(spacePos + 1) : "";

    for (const auto &entry : aiDispatch) {
        if (entry.name == cmd) {
            entry.handler(client, server, args);
            return;
        }
    }
    server.getSocket().sendMessage(client.getFd(), "ko\n", 3);
}

void AiCommands::forward(Client &client, Server &server, const std::string &args)
{
    (void)client;
    (void)server;
    (void)args;
}

void AiCommands::right(Client &client, Server &server, const std::string &args)
{
    (void)client;
    (void)server;
    (void)args;
}

void AiCommands::left(Client &client, Server &server, const std::string &args)
{
    (void)client;
    (void)server;
    (void)args;
}

void AiCommands::look(Client &client, Server &server, const std::string &args)
{
    (void)client;
    (void)server;
    (void)args;
}

void AiCommands::inventory(Client &client, Server &server, const std::string &args)
{
    (void)client;
    (void)server;
    (void)args;
}

void AiCommands::broadcast(Client &client, Server &server, const std::string &args)
{
    (void)client;
    (void)server;
    (void)args;
}

void AiCommands::connectNbr(Client &client, Server &server, const std::string &args)
{
    (void)client;
    (void)server;
    (void)args;
}

void AiCommands::fork(Client &client, Server &server, const std::string &args)
{
    (void)client;
    (void)server;
    (void)args;
}

void AiCommands::eject(Client &client, Server &server, const std::string &args)
{
    (void)client;
    (void)server;
    (void)args;
}

void AiCommands::take(Client &client, Server &server, const std::string &object)
{
    (void)client;
    (void)server;
    (void)object;
}

void AiCommands::set(Client &client, Server &server, const std::string &object)
{
    (void)client;
    (void)server;
    (void)object;
}

void AiCommands::incantation(Client &client, Server &server, const std::string &args)
{
    (void)client;
    (void)server;
    (void)args;
}

}
