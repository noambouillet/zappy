/*
** EPITECH PROJECT, 2026
** Socket
** File description:
** Encapsulation of a TCP socket
*/

#include "Socket.hpp"
#include "GuiExceptions.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

Socket::Socket() : _fd(-1) {}

Socket::~Socket()
{
    closeSocket();
}

Socket::Socket(Socket &&other) noexcept : _fd(other._fd)
{
    other._fd = -1;
}

Socket &Socket::operator=(Socket &&other) noexcept
{
    if (this != &other) {
        closeSocket();
        _fd = other._fd;
        other._fd = -1;
    }
    return *this;
}

void Socket::openSocket()
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0) {
        throw GuiException("socket creation failed.");
    }
}

void Socket::connectSocket(const std::string &ip, unsigned int port)
{
    struct sockaddr_in address;

    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(static_cast<uint16_t>(port));

    if (inet_pton(AF_INET, ip.c_str(), &address.sin_addr) <= 0) {
        throw GuiException("inet_pton failed: Invalid IP address.");
    }
    if (connect(_fd, reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) < 0) {
        closeSocket();
        throw GuiException("connect failed.");
    }
}

void Socket::sendMessage(int fd, const char *buffer, std::size_t size)
{
    std::size_t totalSent = 0;

    while (totalSent < size) {
        ssize_t sentBytes = send(fd, buffer + totalSent, size - totalSent, 0);
        if (sentBytes < 0) {
            throw GuiException("send failed.");
        }
        totalSent += static_cast<std::size_t>(sentBytes);
    }
}

ssize_t Socket::receiveMessage(int fd, char *buffer, std::size_t size)
{
    ssize_t receivedBytes = recv(fd, buffer, size, 0);
    if (receivedBytes < 0) {
        throw GuiException("recv failed.");
    }
    return receivedBytes;
}

void Socket::closeSocket()
{
    if (_fd >= 0) {
        close(_fd);
        _fd = -1;
    }
}

int Socket::getFd() const
{
    return _fd;
}