/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** NetworkHandler
*/

#ifndef NETWORKHANDLER_HPP_
#define NETWORKHANDLER_HPP_

#include <string>
#include <exception>
#include "Socket.hpp"

class NetworkHandler {
    public:
        NetworkHandler(int port, std::string ip);
        ~NetworkHandler();

        int connect_to_server();
        void send_command(int fd, const std::string& msg);
        std::string read_from_server(int fd);
        bool extract_message(std::string &buffer, std::string &message);

    private:
        int _port;
        std::string _ip;
        Socket _socket;
};

#endif /* !NETWORKHANDLER_HPP_ */