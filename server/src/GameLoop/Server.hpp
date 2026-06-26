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
#include <functional>

namespace ZappyServer {

static constexpr int BUFFER_SIZE = 4096;
static constexpr unsigned int RESOURCE_REFILL_TICKS = 20;

struct PendingIncantation {
    std::vector<int> participantFds;
    unsigned int x;
    unsigned int y;
    unsigned int level;
    unsigned int remainingTicks;
};

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
        std::vector<Egg> _eggs;
        std::vector<PendingIncantation> _incantations;
        unsigned int _nextEggId;
        unsigned int _resourceRefillTicks;
        std::chrono::time_point<std::chrono::steady_clock> _startTime;
        std::chrono::time_point<std::chrono::steady_clock> _lastTick;
        bool _running;
        bool _paused;
        std::mt19937 _rng;
        InputParser _shell;

        void setup();
        void processTicks(int ticks);
        void processPendingTicks();
        void processIncantationsTick();
        std::vector<std::reference_wrapper<Client>> getIncantationParticipants(const PendingIncantation &pendingIncantation, bool &success);
        void handleSuccessfulIncantation(const PendingIncantation &pendingIncantation,
            std::vector<std::reference_wrapper<Client>> &participants, Tile &tile);
        void handleFailedIncantation(const PendingIncantation &pendingIncantation);
        void processIncantation(PendingIncantation &pendingIncantation);
        int calculateNextTimeout(double elapsedMs, double tickDurationMs);
        bool assignAiSpawnPosition(Client &client, const std::string &requestedTeamName, unsigned int aliveCount);
        void finalizeAiHandshake(Client &client, const std::string &requestedTeamName, unsigned int aliveCount);
        void updateMinTicksFromClient(int &minTicks, const Client &client) const;
        void updateMinTicksFromIncantations(int &minTicks) const;
        void processFoodDecay(Client &client, PlayerData &player);
        void processClientCommand(Client &client, PlayerData &player);
        void startQueuedCommands(Client &client, PlayerData &player);
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
        bool isPaused() const;
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
        std::vector<PendingIncantation> &getIncantations();
        unsigned int addEgg(const std::string &teamName, unsigned int x, unsigned int y);
        void destroyEggsOnTile(unsigned int x, unsigned int y);
        void killClient(int fd);
};

}

#endif /* !SERVER_HPP_ */
