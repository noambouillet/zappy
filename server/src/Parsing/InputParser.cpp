/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** InputParser
*/

#include "InputParser.hpp"
#include "ServerExceptions.hpp"
#include "Server.hpp"

namespace ZappyServer {

InputParser::InputParser(Server& server) : _server(server) {}

void InputParser::printHelpShell()
{
    std::cout << "COMMANDS:\n" << "\t/help\t\t gives you the list of the shell's commands\n"
        << "\t/exit\t\t lets you leave the shell\n" << "\t/status\t\t displays the status of the teams\n";
}

void InputParser::processCommand(const std::string& command)
{
    if (command.empty())
        return;
    if (command == "/exit") {
        _server.stop();
    } else if (command == "/help") {
        printHelpShell();
    } else if (command == "/status") {
        (void)_server;
        //_server.status();
        std::cout << "Commands not implemented yet.\n";
    } else {
        throw ServerException("Invalid command.");
    }
}

}
