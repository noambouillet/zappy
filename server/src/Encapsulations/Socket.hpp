/*
** EPITECH PROJECT, 2026
** Socket
** File description:
** Encapsulation of a TCP socket
*/

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <cstddef>
#include <sys/socket.h>

namespace ZappyServer {

class Socket {
    private:
        int _fd;
    public:
        Socket();
        ~Socket();

        Socket(const Socket &other) = delete;
        Socket &operator=(const Socket &other) = delete;
        Socket(Socket &&other);
        Socket &operator=(Socket &&other);
        void openSocket();
        void setReuseAddress();
        void bindSocket(unsigned int port);
        void startListening(int backlog);
        int acceptClient();
        void sendMessage(int fd, const char *buffer, std::size_t size);
        ssize_t receiveMessage(int fd, char *buffer, std::size_t size);
        void closeSocket();
        int getFd();
};

}

#endif /* !SOCKET_HPP_ */
