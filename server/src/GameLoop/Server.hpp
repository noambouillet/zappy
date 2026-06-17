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
#include "Egg.hpp"
#include "InputParser.hpp"
#include <vector>
#include <string>
#include <chrono>
#include <random>

namespace ZappyServer {

static constexpr unsigned int BUFFER_SIZE = 4096;
static constexpr unsigned int CELL_SIZE = 20;

class Server {
    private:
        unsigned int _port;
        unsigned int _width;
        unsigned int _height;
        unsigned int _clientsNb;
        unsigned int _freq;
        std::vector<std::string> _teamNames;
        std::vector<std::vector<unsigned int>> _heatmap;
        Socket _socket;
        Poll _poll;
        std::vector<Client> _clients;
        Map _map;
        std::vector<Egg> _eggs;
        unsigned int _nextEggId;
        std::chrono::time_point<std::chrono::steady_clock> _lastTick;
        bool _running;
        bool _paused;
        std::mt19937 _rng;
        InputParser _shell;

        void setup();
        void processTicks(int ticks);
        int calculateNextTimeout(double elapsedMs, double tickDurationMs);
        void processFoodDecay(Client &client, PlayerData &player);
        void processClientCommand(Client &client, PlayerData &player);
        void acceptClient();
        void dispatchClientLine(Client &client, const std::string &completeLine);
        void readClient(Client &client);
        void handleHandshake(Client &client, const std::string &line);
        void handleGuiHandshake(Client &client);
        unsigned int countAlivePlayersInTeam(const std::string &teamName) const;
        int computeAvailableSlots(const std::string &teamName, unsigned int aliveCount) const;
        void handleAiHandshake(Client &client, const std::string &requestedTeamName);
        void closeClients();
        void disconnectClient(Client &client);
        bool isValidTeam(const std::string &name) const;
        void acceptPendingClients(const std::vector<pollfd> &fds);
        void readClients(const std::vector<pollfd> &fds);
        void readShellCommands(const std::vector<pollfd>& fds);
        void removeDeadClients();
    public:
        Server(unsigned int port, unsigned int width, unsigned int height, unsigned int clientsNb,
            unsigned int freq, unsigned int seed, const std::vector<std::string> &teamNames);
        ~Server();
        void run();
        void stop();
        void pause();
        void resume();
        Socket &getSocket();
        unsigned int getWidth() const;
        unsigned int getHeight() const;
        Map &getMap();
        const std::vector<std::string> &getTeamNames() const;
        unsigned int getFreq() const;
        void setFreq(unsigned int t);
        std::vector<Client> &getClients();
        unsigned int getClientsNb() const;
        const std::vector<Egg> &getEggs() const;
        void visitTile(unsigned int x, unsigned int y);
        void saveHeatmap(const std::string &filename) const;
        unsigned int addEgg(const std::string &teamName, unsigned int x, unsigned int y);
        void destroyEggsOnTile(unsigned int x, unsigned int y);
        void killClient(int fd);
};

}

#endif /* !SERVER_HPP_ */
