/*
** EPITECH PROJECT, 2026
** main.cpp
** File description:
** Main of the server
*/

#include <exception>
#include <iostream>
#include "ServerData.hpp"

int main(int argc, char **argv)
{
    try {
        ZappyServer::ServerData server;

        server.parse(argc, argv);
        server.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
