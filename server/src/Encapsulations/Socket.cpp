/*
** EPITECH PROJECT, 2026
** Socket
** File description:
** Encapsulation of a TCP socket
*/

#include "Socket.hpp"
#include "ServerExceptions.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace ZappyServer {

Socket::Socket() : _fd(-1)
{
}

Socket::~Socket()
{
    closeSocket();
}

Socket::Socket(Socket &&other) : _fd(other._fd)
{
    other._fd = -1;
}

Socket &Socket::operator=(Socket &&other)
{
    if (this == &other) {
        return *this;
    }
    closeSocket();
    _fd = other._fd;
    other._fd = -1;
    return *this;
}

void Socket::openSocket()
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0) {
        throw FatalSocketException("socket creation failed.");
    }
}

void Socket::setReuseAddress()
{
    int option = 1;

    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
        throw FatalSocketException("setsockopt failed.");
    }
}

void Socket::bindSocket(unsigned int port)
{
    struct sockaddr_in address;

    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(static_cast<uint16_t>(port));
    if (bind(_fd, reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) < 0) {
        throw FatalSocketException("Bind failed, try a different port.");
    }
}

void Socket::startListening(int backlog)
{
    if (listen(_fd, backlog) < 0) {
        throw FatalSocketException("listen failed.");
    }
}

int Socket::acceptClient()
{
    int clientFd = accept(_fd, nullptr, nullptr);

    if (clientFd < 0) {
        throw SocketException("accept failed.");
    }
    return clientFd;
}

void Socket::sendMessage(int fd, const char *buffer, std::size_t size)
{
    std::size_t totalSent = 0;

    while (totalSent < size) {
        ssize_t bytesSent = send(fd, buffer + totalSent, size - totalSent, 0);
        if (bytesSent < 0) {
            throw SocketException("send failed.");
        }
        totalSent += static_cast<std::size_t>(bytesSent);
    }
}

ssize_t Socket::receiveMessage(int fd, char *buffer, std::size_t size)
{
    ssize_t bytesReceived = recv(fd, buffer, size, 0);
    if (bytesReceived < 0) {
        throw SocketException("recv failed.");
    }
    return bytesReceived;
}

void Socket::closeSocket()
{
    if (_fd >= 0) {
        close(_fd);
        _fd = -1;
    }
}

int Socket::getFd()
{
    return _fd;
}

}
