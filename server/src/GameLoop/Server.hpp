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

namespace ZappyServer {

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
        void handleHandshake(Client &client, const std::string &line);
        void handleGuiHandshake(Client &client);
        void handleAiHandshake(Client &client, const std::string &requestedTeamName);
        void closeClients();
        void disconnectClient(Client &client);
        bool isValidTeam(const std::string &name) const;
        void acceptPendingClients(const std::vector<pollfd> &fds);
        void readClients(const std::vector<pollfd> &fds);
        void removeDeadClients();
    public:
        Server(unsigned int port, unsigned int width, unsigned int height, unsigned int clientsNb,
            unsigned int freq, const std::vector<std::string> &teamNames);
        ~Server();
        void run();
        Socket &getSocket();
        unsigned int getWidth() const;
        unsigned int getHeight() const;
        Map &getMap();
        const std::vector<std::string> &getTeamNames() const;
        unsigned int getFreq() const;
        void setFreq(unsigned int t);
        std::vector<Client> &getClients();
};

}

#endif /* !SERVER_HPP_ */
