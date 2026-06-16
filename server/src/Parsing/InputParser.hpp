/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** InputParser
*/

#ifndef INPUTPARSER_HPP_
#define INPUTPARSER_HPP_

#include <string>
#include <iostream>

namespace ZappyServer {

class Server;

class InputParser {
    private:
        Server& _server;

        void printHelpShell();
    public:
        InputParser(Server& server);
        ~InputParser() = default;
        void processCommand(const std::string& command);
};

}

#endif /* !INPUTPARSER_HPP_ */
