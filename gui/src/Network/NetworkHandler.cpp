/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** NetworkHandler
*/

#include "NetworkHandler.hpp"
#include "GuiExceptions.hpp"
#include <iostream>

NetworkHandler::NetworkHandler(int port, std::string ip): _port(port), _ip(ip) {}

NetworkHandler::~NetworkHandler() {}

int NetworkHandler::connect_to_server()
{
    try {
        _socket.openSocket();
        _socket.connectSocket(_ip, _port);
    } catch (const std::exception &e) {
        throw GuiException(std::string("Connection fail: ") + e.what());
    }
    return _socket.getFd();
}

void NetworkHandler::send_command(int fd, const std::string& msg)
{
    std::string command = msg + "\n";
    try {
        _socket.sendMessage(fd, command.c_str(), command.size());
    } catch (const std::exception &e) {
        throw GuiException(std::string("Write fail: ") + e.what());
    }
}

std::string NetworkHandler::read_from_server(int fd)
{
    char buffer[4096];
    ssize_t bytes = 0;

    try {
        bytes = _socket.receiveMessage(fd, buffer, sizeof(buffer) - 1);
    } catch (const std::exception &e) {
        throw GuiException(std::string("Read fail: ") + e.what());
    }
    
    if (bytes == 0)
        throw GuiException("connection closed");
        
    buffer[bytes] = '\0';
    return std::string(buffer, bytes);
}

bool NetworkHandler::extract_message(std::string &buffer, std::string &message)
{
    std::size_t pos = buffer.find("\n");

    if (pos == std::string::npos)
        return false;
    message = buffer.substr(0, pos);
    if (!message.empty() && message.back() == '\r') {
        message.pop_back();
    }
    buffer.erase(0, pos + 1);
    return true;
}