/*
** EPITECH PROJECT, 2026
** Server
** File description:
** Minimal server loop
*/

#include "Server.hpp"
#include "Tile.hpp"
#include "Logger.hpp"
#include "GuiCommands.hpp"
#include "AiCommands.hpp"
#include <algorithm>
#include <csignal>

namespace ZappyServer {

// 🚀 Server starting up !!!!

Server::Server(unsigned int port, unsigned int width, unsigned int height, unsigned int clientsNb, unsigned int freq, unsigned int seed, const std::vector<std::string> &teamNames)
    : _port(port), _width(width), _height(height), _clientsNb(clientsNb), _freq(freq), _teamNames(teamNames), _map(width, height),
    _nextEggId(0), _running(true), _paused(false), _shell(*this)
{
    _map.setSeed(seed);
    _map.generate();
    _rng.seed(seed);
}

Server::~Server()
{
    closeClients();
}

Socket &Server::getSocket() {
    return _socket;
}

void Server::stop()
{
    _running = false;
}

void Server::pause()
{
    _paused = true;
    std::cout << "The world is now paused.\n";
}

void Server::resume()
{
    if (_paused) {
        _paused = false;
        _lastTick = std::chrono::steady_clock::now();
        std::cout << "The world is now back running.\n";
    }
}

void Server::setup()
{
    _socket.openSocket();
    _socket.setReuseAddress();
    _socket.bindSocket(_port);
    logger.write("The world has opened on port " + std::to_string(_port) + ".");
    _socket.startListening(128);
    _poll.addFd(_socket.getFd(), POLLIN);
    if (isatty(STDIN_FILENO)) {
        std::cout << "> " << std::flush;
        _poll.addFd(STDIN_FILENO, POLLIN);
    }
    _lastTick = std::chrono::steady_clock::now();

    signal(SIGPIPE, SIG_IGN);
}

void Server::acceptClient()
{
    int newClientFd = _socket.acceptClient();
    _clients.emplace_back(newClientFd);
    _poll.addFd(newClientFd, POLLIN);
    _socket.sendMessage(newClientFd, "WELCOME\n", 8);
    logger.info("New connection from fd " + std::to_string(newClientFd));
}

bool Server::isValidTeam(const std::string &name) const
{
    return std::find(_teamNames.begin(), _teamNames.end(), name) != _teamNames.end();
}

void Server::disconnectClient(Client &client)
{
    logger.info("Client fd " + std::to_string(client.getFd()) + " disconnected");
    _poll.removeFd(client.getFd());
    close(client.getFd());
    client.invalidate();
}

void Server::killClient(int fd)
{
    for (Client &client : _clients) {
        if (client.isDead())
            continue;
        if (client.getFd() != fd)
            continue;
        if (client.getState() != ClientState::AI) {
            std::cout << "Only AI clients can be killed.\n";
            return;
        }
        GuiCommands::pdi(*this, fd);
        _socket.sendMessage(client.getFd(), "dead\n", 5);
        disconnectClient(client);
        logger.write("Murdered AI client " + std::to_string(fd) + ".");
        return;
    }
    std::cout << "Client not found.\n";
}

void Server::handleGuiHandshake(Client &client)
{
    client.setState(ClientState::GUI);
    client.setTeamName("GRAPHIC");
    std::string mapSize = "msz " + std::to_string(_width) + " " + std::to_string(_height) + "\n";
    std::string timeUnit = "sgt " + std::to_string(_freq) + "\n";
    _socket.sendMessage(client.getFd(), mapSize.c_str(), mapSize.size());
    _socket.sendMessage(client.getFd(), timeUnit.c_str(), timeUnit.size());
    for (unsigned int tileY = 0; tileY < _height; tileY++) {
        for (unsigned int tileX = 0; tileX < _width; tileX++) {
            Tile &tile = _map.getTile(tileX, tileY);
            std::string tileContent = "bct " + std::to_string(tileX) + " " + std::to_string(tileY);
            for (int resourceIndex = 0; resourceIndex < 7; resourceIndex++)
                tileContent += " " + std::to_string(tile.resources[resourceIndex]);
            tileContent += "\n";
            _socket.sendMessage(client.getFd(), tileContent.c_str(), tileContent.size());
        }
    }
    for (const std::string &teamName : _teamNames) {
        std::string teamNameMessage = "tna " + teamName + "\n";
        _socket.sendMessage(client.getFd(), teamNameMessage.c_str(), teamNameMessage.size());
    }
    logger.write("The omniscient graphic team has entered the world.");
}

unsigned int Server::countAlivePlayersInTeam(const std::string &teamName) const
{
    unsigned int aliveCount = 0;
    for (const Client &otherClient : _clients) {
        if (!otherClient.isDead() && otherClient.getState() == ClientState::AI && otherClient.getTeamName() == teamName) {
            aliveCount++;
        }
    }
    return aliveCount;
}

int Server::computeAvailableSlots(const std::string &teamName, unsigned int aliveCount) const
{
    unsigned int totalSlots = _clientsNb;
    for (const Egg &egg : _eggs) {
        if (egg.teamName == teamName)
            totalSlots++;
    }
    int remaining = static_cast<int>(totalSlots) - static_cast<int>(aliveCount) - 1;
    if (remaining < 0)
        return 0;
    return remaining;
}

void Server::handleAiHandshake(Client &client, const std::string &requestedTeamName)
{
    if (!isValidTeam(requestedTeamName)) {
        _socket.sendMessage(client.getFd(), "ko\n", 3);
        disconnectClient(client);
        logger.write("A fake team tried to join the world, in the name of " + requestedTeamName + ".");
        return;
    }

    unsigned int aliveCount = countAlivePlayersInTeam(requestedTeamName);
    client.initPlayerData();

    if (aliveCount >= _clientsNb) {
        auto eggIt = std::find_if(_eggs.begin(), _eggs.end(),
            [&requestedTeamName](const Egg &e) { return e.teamName == requestedTeamName; });
        if (eggIt == _eggs.end()) {
            _socket.sendMessage(client.getFd(), "ko\n", 3);
            disconnectClient(client);
            return;
        }
        GuiCommands::ebo(*this, eggIt->id);
        client.getPlayerData()->setX(eggIt->x);
        client.getPlayerData()->setY(eggIt->y);
        _eggs.erase(eggIt);
    } else {
        std::uniform_int_distribution<unsigned int> distX(0, _width - 1);
        std::uniform_int_distribution<unsigned int> distY(0, _height - 1);

        client.getPlayerData()->setX(distX(_rng));
        client.getPlayerData()->setY(distY(_rng));
    }

    client.setState(ClientState::AI);
    client.setTeamName(requestedTeamName);
    client.getPlayerData()->setDirection((rand() % 4) + 1);
    client.getPlayerData()->setLevel(1);
    client.getPlayerData()->setInventory(0, 10);

    int remaining = computeAvailableSlots(requestedTeamName, aliveCount);
    std::string availableSlots  = std::to_string(remaining) + "\n";
    std::string worldDimensions = std::to_string(_width) + " " + std::to_string(_height) + "\n";
    _socket.sendMessage(client.getFd(), availableSlots.c_str(), availableSlots.size());
    _socket.sendMessage(client.getFd(), worldDimensions.c_str(), worldDimensions.size());
    GuiCommands::pnw(*this, client.getFd());
    logger.write("The wanderer team " + requestedTeamName + " has entered the world.");
}

void Server::handleHandshake(Client &client, const std::string &receivedTeamName)
{
    if (receivedTeamName == "GRAPHIC") {
        handleGuiHandshake(client);
        return;
    }
    handleAiHandshake(client, receivedTeamName);
}

void Server::dispatchClientLine(Client &client, const std::string &completeLine)
{
    if (client.getState() == ClientState::WAITING_TEAM) {
        handleHandshake(client, completeLine);
    } else if (client.getState() == ClientState::GUI) {
        GuiCommands::dispatch(client, *this, completeLine);
    } else if (client.getState() == ClientState::AI) {
        std::size_t spacePos = completeLine.find(' ');
        std::string cmdName = completeLine.substr(0, spacePos);
        unsigned int ticks = AiCommands::getCommandTicks(cmdName);
        client.getPlayerData()->queueCommand(completeLine, ticks);
    }
}

void Server::readClient(Client &client)
{
    char readChunk[BUFFER_SIZE];
    ssize_t receivedBytes = _socket.receiveMessage(client.getFd(), readChunk, BUFFER_SIZE);

    if (receivedBytes <= 0) {
        disconnectClient(client);
        return;
    }

    client.getReadBuffer().append(readChunk, receivedBytes);
    std::string &readBuffer = client.getReadBuffer();
    std::size_t newlinePos;

    while ((newlinePos = readBuffer.find('\n')) != std::string::npos) {
        std::string completeLine = readBuffer.substr(0, newlinePos);
        readBuffer.erase(0, newlinePos + 1);

        if (!completeLine.empty() && completeLine.back() == '\r') {
            completeLine.pop_back();
        }
        if (completeLine.empty()) {
            continue;
        }

        dispatchClientLine(client, completeLine);
    }
}

void Server::closeClients()
{
    for (Client &client : _clients) {
        if (!client.isDead())
            close(client.getFd());
    }
    _clients.clear();
    if (_socket.getFd() >= 0)
        close(_socket.getFd());
}

void Server::readClients(const std::vector<pollfd> &fds)
{
    for (const pollfd &entry : fds) {
        if (!(entry.revents & POLLIN)) {
            continue;
        }
        for (Client &client : _clients) {
            if (!client.isDead() && client.getFd() == entry.fd) {
                readClient(client);
            }
        }
    }
}

void Server::removeDeadClients()
{
    std::vector<Client> alive;
    for (Client &client : _clients) {
        if (!client.isDead())
            alive.push_back(std::move(client));
    }
    _clients = std::move(alive);
}

void Server::acceptPendingClients(const std::vector<pollfd> &fds)
{
    for (const pollfd &entry : fds) {
        if (entry.fd == _socket.getFd() && (entry.revents & POLLIN)) {
            acceptClient();
        }
    }
}

void Server::processFoodDecay(Client &client, PlayerData &player)
{
    if (player.getFoodTicks() > 0) {
        player.setFoodTicks(player.getFoodTicks() - 1);
    }

    if (player.getFoodTicks() == 0) {
        if (player.getInventory(0) > 0) {
            player.setInventory(0, player.getInventory(0) - 1);
            player.setFoodTicks(126);
        } else {
            GuiCommands::pdi(*this, client.getFd());
            _socket.sendMessage(client.getFd(), "dead\n", 5);
            logger.write("A wanderer starved to death...");
            disconnectClient(client);
        }
    }
}

void Server::processClientCommand(Client &client, PlayerData &player)
{
    if (client.isDead() || player.getCommandQueue().empty()) {
        return;
    }

    QueuedCommand &cmd = player.getCommandQueue().front();
    if (cmd.remainingTicks > 0) {
        cmd.remainingTicks--;
    }
    if (cmd.remainingTicks == 0) {
        logger.info("Player fd " + std::to_string(client.getFd()) + " executed command: " + cmd.line);
        AiCommands::dispatch(client, *this, cmd.line);
        player.getCommandQueue().pop();
    }
}

void Server::processTicks(int ticks)
{
    for (int index = 0; index < ticks; index++) {
        for (Client &client : _clients) {
            if (client.isDead() || client.getState() != ClientState::AI || !client.getPlayerData().has_value()) {
                continue;
            }
            PlayerData &player = client.getPlayerData().value();

            processFoodDecay(client, player);
            if (client.isDead())
                continue;
            processClientCommand(client, player);
        }
    }
}

int Server::calculateNextTimeout(double elapsedMs, double tickDurationMs)
{
    int minTicks = -1;
    for (const Client &client : _clients) {
        if (client.isDead() || client.getState() != ClientState::AI || !client.getPlayerData().has_value()) {
            continue;
        }
        const PlayerData &player = client.getPlayerData().value();

        if (player.getFoodTicks() > 0) {
            int eventTicks = player.getFoodTicks();
            if (minTicks == -1 || eventTicks < minTicks) {
                minTicks = eventTicks;
            }
        }
        if (!player.getCommandQueue().empty()) {
            const QueuedCommand &cmd = player.getCommandQueue().front();
            int eventTicks = cmd.remainingTicks > 0 ? cmd.remainingTicks : 1;
            if (minTicks == -1 || eventTicks < minTicks) {
                minTicks = eventTicks;
            }
        }
    }

    if (minTicks != -1) {
        double targetElapsedMs = minTicks * tickDurationMs;
        if (elapsedMs < targetElapsedMs) {
            return std::ceil(targetElapsedMs - elapsedMs);
        } else {
            return 0;
        }
    }
    return -1;
}

void Server::readShellCommands(const std::vector<pollfd>& fds)
{
    for (const pollfd &entry : fds) {
        if (entry.fd == STDIN_FILENO && (entry.revents & POLLIN)) {
            std::string line;

            if (!std::getline(std::cin, line)) {
                stop();
                return;
            }
            try {
                _shell.processCommand(line);
            } catch (const std::exception &e) {
                std::cerr << e.what() << '\n';
            }
            if (_running)
                std::cout << "> " << std::flush;
        }
    }
}

void Server::run()
{
    setup();
    while (_running) {
        int timeout = -1;
        if (!_paused) {
            double tickDurationMs = 1000.0 / _freq;
            auto now = std::chrono::steady_clock::now();
            double elapsedMs = std::chrono::duration<double, std::milli>(now - _lastTick).count();
            int ticksToProcess = elapsedMs / tickDurationMs;

            if (ticksToProcess > 0) {
                processTicks(ticksToProcess);
                _lastTick += std::chrono::milliseconds((int)(ticksToProcess * tickDurationMs));
                now = std::chrono::steady_clock::now();
                elapsedMs = std::chrono::duration<double, std::milli>(now - _lastTick).count();
            }

            timeout = calculateNextTimeout(elapsedMs, tickDurationMs);
        }
        int ret = _poll.wait(timeout);
        if (ret > 0) {
            const std::vector<pollfd> &fds = _poll.getFds();
            acceptPendingClients(fds);
            readClients(fds);
            readShellCommands(fds);
            removeDeadClients();
        }
    }
}

unsigned int Server::getWidth() const
{
    return _width;
}

unsigned int Server::getHeight() const
{
    return _height;
}

Map &Server::getMap()
{
    return _map;
}

const std::vector<std::string> &Server::getTeamNames() const
{
    return _teamNames;
}

unsigned int Server::getFreq() const
{
    return _freq;
}

void Server::setFreq(unsigned int t)
{
    _freq = t;
    _lastTick = std::chrono::steady_clock::now();
}

std::vector<Client> &Server::getClients()
{
    return _clients;
}

unsigned int Server::getClientsNb() const
{
    return _clientsNb;
}

const std::vector<Egg> &Server::getEggs() const
{
    return _eggs;
}

unsigned int Server::addEgg(const std::string &teamName, unsigned int x, unsigned int y)
{
    unsigned int id = _nextEggId++;
    _eggs.push_back({id, teamName, x, y});
    return id;
}

void Server::destroyEggsOnTile(unsigned int x, unsigned int y)
{
    auto it = std::remove_if(_eggs.begin(), _eggs.end(),
        [this, x, y](const Egg &egg) {
            if (egg.x == x && egg.y == y) {
                GuiCommands::edi(*this, egg.id);
                return true;
            }
            return false;
        });
    _eggs.erase(it, _eggs.end());
}

}
