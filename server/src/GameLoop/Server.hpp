/*
** EPITECH PROJECT, 2026
** Server
** File description:
** Minimal server loop
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "Poll.hpp"
#include "Socket.hpp"
#include <vector>

class Server {
    private:
        unsigned int _port;
        Socket _socket;
        Poll _poll;
        std::vector<int> _clientFds;

        void setup();
        void acceptClient();
        void closeClients();
    public:
        explicit Server(unsigned int port);
        ~Server();
        void run();
};

#endif /* !SERVER_HPP_ */
