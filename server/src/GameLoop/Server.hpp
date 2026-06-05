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
#include "Client.hpp"
#include "Map.hpp"
#include <vector>
#include <string>

class Server {
    private:
        unsigned int _port;
        unsigned int _width;
        unsigned int _height;
        unsigned int _clientsNb;
        unsigned int _freq;
        std::vector<std::string> _teamNames;
        Socket _socket;
        Poll _poll;
        std::vector<Client> _clients;
        Map _map;

        void setup();
        void acceptClient();
        void readClient(Client &client);
        void handleHandshake(Client &client, std::string &line);
        void handleGuiHandshake(Client &client);
        void handleAiHandshake(Client &client, std::string &requestedTeamName);
        void closeClients();
        void disconnectClient(Client &client);
        bool isValidTeam(std::string &name) const;
    public:
        Server(unsigned int port, unsigned int width, unsigned int height, unsigned int clientsNb,
            unsigned int freq, const std::vector<std::string> &teamNames);
        ~Server();
        void run();
};

#endif /* !SERVER_HPP_ */
