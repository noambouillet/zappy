/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** NetworkHandler
*/

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#ifndef NETWORKHANDLER_HPP_
#define NETWORKHANDLER_HPP_

class NetworkHandler {
    public:
        NetworkHandler(int port, std::string ip);
        ~NetworkHandler();
        int connect_to_server();
        void send_command(int fd, const std::string& msg);
        std::string read_from_server(int fd);
        bool extract_message(std::string &buffer, std::string &message);
        class NETWORK_ERROR : public std::exception {
            public:
                NETWORK_ERROR(std::string error);
                const char *what() const noexcept override;
            private:
                std::string _msg;
        };
    private:
        int _port;
        std::string _ip;
};

#endif /* !NetworkHandler_HPP_ */
