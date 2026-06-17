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
        << "\t/exit\t\t lets you leave the shell\n" << "\t/clear\t\t lets you clear the screen.\n"
        << "\t/pause\t\t stops the gameloop and doesn't compute ticks.\n" << "\t/resume\t\t resumes the gameloop.\n";
}

void InputParser::processCommand(const std::string& command)
{
    if (command.empty())
        return;
    if (command == "/exit") {
        _server.stop();
    } else if (command == "/help") {
        printHelpShell();
    } else if (command == "/clear") {
        std::cout << "\033[2J\033[H";
    } else if (command == "/pause") {
        _server.pause();
    } else if (command == "/resume") {
        _server.resume();
    } else {
        throw ServerException("Invalid command.");
    }
}

}
