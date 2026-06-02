/*
** EPITECH PROJECT, 2026
** Server
** File description:
** Minimal server loop
*/

#include "Server.hpp"
#include <poll.h>
#include <unistd.h>

Server::Server(unsigned int port) : _port(port)
{
}

Server::~Server()
{
    closeClients();
}

void Server::setup()
{
    _socket.openSocket();
    _socket.setReuseAddress();
    _socket.bindSocket(_port);
    _socket.startListening(128);
    _poll.addFd(_socket.getFd(), POLLIN);
}

void Server::acceptClient()
{
    int clientFd = _socket.acceptClient();
    _socket.sendMessage(clientFd, "WELCOME\n", 8);
    _clientFds.push_back(clientFd);
}

void Server::closeClients()
{
    for (int clientFd : _clientFds)
        close(clientFd);
    _clientFds.clear();
}

void Server::run()
{
    setup();
    while (true) {
        int readyCount = _poll.wait();
        const std::vector<struct pollfd> &fds = _poll.getFds();

        if (readyCount <= 0)
            continue;
        for (const struct pollfd &pollDescriptor : fds) {
            if (pollDescriptor.fd == _socket.getFd() && (pollDescriptor.revents & POLLIN) != 0)
                acceptClient();
        }
    }
}
