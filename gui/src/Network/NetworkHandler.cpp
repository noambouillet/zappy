/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** NetworkHandler
*/

#include "NetworkHandler.hpp"

NetworkHandler::NetworkHandler(int port, std::string ip): _port(port), _ip(ip) {}

NetworkHandler::~NetworkHandler() {}

NetworkHandler::NETWORK_ERROR::NETWORK_ERROR(std::string error) : _msg(error) {}

const char *NetworkHandler::NETWORK_ERROR::what() const noexcept
{
    return _msg.c_str();
}

int NetworkHandler::connect_to_server()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        throw NETWORK_ERROR("socket fail");
    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);

    if (inet_pton(AF_INET, _ip.c_str(), &addr.sin_addr) <= 0)
        throw NETWORK_ERROR("inet_pton fail");
    if (connect(fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        close(fd);
        throw NETWORK_ERROR("connect fail");
    }
    return fd;
}

void NetworkHandler::send_command(int fd, const std::string& msg)
{
    std::string command = msg + "\r\n";
    ssize_t total = 0;
    ssize_t size = command.size();

    while (total < size) {
        ssize_t sent = write(fd, command.c_str() + total, size - total);
        if (sent < 0)
            throw NETWORK_ERROR("write failed");
        total += sent;
    }
    return;
}

std::string NetworkHandler::read_from_server(int fd)
{
    char buffer[4096];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    
    if (bytes < 0)
        throw NETWORK_ERROR("read failed");
    if (bytes == 0)
        throw NETWORK_ERROR("connection closed");
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