/*
** EPITECH PROJECT, 2026
** main.cpp
** File description:
** Main of the server
*/

#include <exception>
#include <iostream>
#include "ServerData.hpp"
#include "Logger.hpp"

int main(int argc, char **argv)
{
    try {
        ZappyServer::ServerData server;
        server.parse(argc, argv);
        server.run();
    } catch (const std::exception& e) {
        ZappyServer::logger.error(e.what());
        return 84;
    }
    return 0;
}
