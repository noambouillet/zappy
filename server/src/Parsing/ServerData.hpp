/*
** EPITECH PROJECT, 2026
** ServerData
** File description:
** Command line parsing and server configuration storage
*/

#ifndef SERVER_DATA_HPP_
#define SERVER_DATA_HPP_

#include <string>
#include <vector>

namespace ZappyServer {

class ServerData {
    private:
        unsigned int _port;
        unsigned int _width;
        unsigned int _height;
        unsigned int _clientsNb;
        unsigned int _freq;
        std::vector<std::string> _teamNames;

        void parseArgs(char **argv, int argc, int &index);
        void printServerData() const;
    public:
        ServerData();

        void parse(int argc, char **argv);
        void run() const;
        unsigned int getPort() const;
        unsigned int getWidth() const;
        unsigned int getHeight() const;
        unsigned int getClientsNb() const;
        unsigned int getFreq() const;
        const std::vector<std::string> &getTeamNames() const;
};

}

#endif /* !SERVER_DATA_HPP_ */
