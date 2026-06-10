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
#include <poll.h>
#include <unistd.h>
#include <algorithm>

namespace ZappyServer {

// #commentaire gen par chatgpt !! C'est une blague, juste je sais que vous allez jamais flag triche, meme si je mets des emojis ultra obvious:
// 🚀 Server starting up mdr

Server::Server(unsigned int port, unsigned int width, unsigned int height, unsigned int clientsNb, unsigned int freq, const std::vector<std::string> &teamNames)
    : _port(port), _width(width), _height(height), _clientsNb(clientsNb), _freq(freq), _teamNames(teamNames), _map(width, height)
{}

Server::~Server()
{
    closeClients();
}

Socket &Server::getSocket() {
    return _socket;
}

void Server::setup()
{
    _socket.openSocket();
    _socket.setReuseAddress();
    _socket.bindSocket(_port);
    logger.write("The world has opened on port " + std::to_string(_port) + ".");
    _socket.startListening(128);
    _poll.addFd(_socket.getFd(), POLLIN);
    _lastTick = std::chrono::steady_clock::now();
}

void Server::acceptClient()
{
    int newClientFd = _socket.acceptClient();
    _clients.emplace_back(newClientFd);
    _poll.addFd(newClientFd, POLLIN);
    _socket.sendMessage(newClientFd, "WELCOME\n", 8);
}

bool Server::isValidTeam(const std::string &name) const
{
    return std::find(_teamNames.begin(), _teamNames.end(), name) != _teamNames.end();
}

void Server::disconnectClient(Client &client)
{
    _poll.removeFd(client.getFd());
    close(client.getFd());
    client.invalidate();
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

void Server::handleAiHandshake(Client &client, const std::string &requestedTeamName)
{
    if (!isValidTeam(requestedTeamName)) {
        _socket.sendMessage(client.getFd(), "ko\n", 3);
        disconnectClient(client);
        logger.write("A fake team tried to join the world, in the name of " + requestedTeamName + ".");
        return;
    }
    client.setState(ClientState::AI);
    client.setTeamName(requestedTeamName);
    client.setX(rand() % _width);
    client.setY(rand() % _height);
    client.setDirection((rand() % 4) + 1);
    client.setLevel(1);
    client.setInventory(0, 10);
    std::string availableSlots  = std::to_string(_clientsNb) + "\n";
    std::string worldDimensions = std::to_string(_width) + " " + std::to_string(_height) + "\n";
    _socket.sendMessage(client.getFd(), availableSlots.c_str(), availableSlots.size());
    _socket.sendMessage(client.getFd(), worldDimensions.c_str(), worldDimensions.size());
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

void Server::readClient(Client &client)
{
    char readChunk[1024];
    ssize_t receivedBytes = _socket.receiveMessage(client.getFd(), readChunk, sizeof(readChunk) - 1);

    if (receivedBytes == 0) {
        disconnectClient(client);
        return;
    }
    readChunk[receivedBytes] = '\0';
    client.getReadBuffer() += readChunk;
    std::string &readBuffer = client.getReadBuffer();
    std::size_t newlinePos;
    while ((newlinePos = readBuffer.find('\n')) != std::string::npos) {
        std::string completeLine = readBuffer.substr(0, newlinePos);
        readBuffer.erase(0, newlinePos + 1);
        if (!completeLine.empty() && completeLine.back() == '\r')
            completeLine.pop_back();
        if (completeLine.empty())
            continue;
        if (client.getState() == ClientState::WAITING_TEAM)
            handleHandshake(client, completeLine);
        else if (client.getState() == ClientState::GUI)
            GuiCommands::dispatch(client, *this, completeLine);
        else if (client.getState() == ClientState::AI) {
            unsigned int ticks = 7;
            std::size_t spacePos = completeLine.find(' ');
            std::string cmdName = completeLine.substr(0, spacePos);
            if (cmdName == "Inventory")
                ticks = 1;
            else if (cmdName == "Connect_nbr")
                ticks = 0;
            else if (cmdName == "Fork")
                ticks = 42;
            else if (cmdName == "Incantation")
                ticks = 300;
            client.queueCommand(completeLine, ticks);
        }
    }
}

void Server::closeClients()
{
    for (Client &client : _clients) {
        if (!client.isDead())
            close(client.getFd());
    }
    _clients.clear();
    close(_socket.getFd());
}

void Server::readClients(const std::vector<pollfd> &fds)
{
    for (const pollfd &entry : fds) {
        if (!(entry.revents & POLLIN))
            continue;
        for (Client &client : _clients) {
            if (!client.isDead() && client.getFd() == entry.fd)
                readClient(client);
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
        if (entry.fd == _socket.getFd() && (entry.revents & POLLIN))
            acceptClient();
    }
}

void Server::processTicks(int ticks)
{
    for (Client &client : _clients) {
        if (client.isDead() || client.getState() != ClientState::AI)
            continue;
        for (int i = 0; i < ticks; i++) {
            if (!client.getCommandQueue().empty()) {
                QueuedCommand &cmd = client.getCommandQueue().front();
                if (cmd.remainingTicks > 0)
                    cmd.remainingTicks--;
                if (cmd.remainingTicks == 0) {
                    AiCommands::dispatch(client, *this, cmd.line);
                    client.getCommandQueue().pop();
                }
            }
        }
    }
}

void Server::run()
{
    setup();
    while (true) {
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
        int timeout = tickDurationMs - elapsedMs;
        if (timeout < 0)
            timeout = 0;
        if (_poll.wait(timeout) <= 0)
            continue;
        const std::vector<pollfd> &fds = _poll.getFds();
        acceptPendingClients(fds);
        readClients(fds);
        removeDeadClients();
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
}

std::vector<Client> &Server::getClients()
{
    return _clients;
}

unsigned int Server::getClientsNb() const
{
    return _clientsNb;
}

}
