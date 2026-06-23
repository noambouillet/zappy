/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** Socket
*/

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <string>
#include <sys/socket.h>

class Socket {
    public:
        Socket();
        ~Socket();
        Socket(Socket &&other) noexcept;
        Socket &operator=(Socket &&other) noexcept;
        Socket(const Socket &other) = delete;
        Socket &operator=(const Socket &other) = delete;
        void openSocket();
        void connectSocket(const std::string &ip, unsigned int port);
        void sendMessage(int fd, const char *buffer, std::size_t size);
        ssize_t receiveMessage(int fd, char *buffer, std::size_t size);
        void closeSocket();
        int getFd() const;

    private:
        int _fd;
};

#endif /* !SOCKET_HPP_ */