/*
** EPITECH PROJECT, 2026
** AI Commands
** File description:
** Implementation of the commands used for the AI
*/

#include "AiCommands.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "GuiCommands.hpp"
#include "Logger.hpp"
#include <array>
#include <string_view>
#include <unordered_map>
#include <cmath>
#include <vector>

namespace ZappyServer {

// 🚀 L'IA il y en a pas mal qui connaissent mdr
int getSoundDirection(int senderX, int senderY, int receiverX, int receiverY, int mapWidth, int mapHeight, unsigned int receiverDir) {
    int deltaXDirect = senderX - receiverX;
    int deltaXWrapped = (senderX > receiverX) ? (senderX - receiverX - mapWidth) : (senderX - receiverX + mapWidth);
    int shortestDeltaX = (std::abs(deltaXDirect) < std::abs(deltaXWrapped)) ? deltaXDirect : deltaXWrapped;
    int deltaYDirect = senderY - receiverY;
    int deltaYWrapped = (senderY > receiverY) ? (senderY - receiverY - mapHeight) : (senderY - receiverY + mapHeight);
    int shortestDeltaY = (std::abs(deltaYDirect) < std::abs(deltaYWrapped)) ? deltaYDirect : deltaYWrapped;
    if (shortestDeltaX == 0 && shortestDeltaY == 0)
        return 0;

    int dxSign = (shortestDeltaX > 0) - (shortestDeltaX < 0);
    int dySign = (shortestDeltaY > 0) - (shortestDeltaY < 0);

    static const int absDirMatrix[3][3] = {
        {8, 1, 2},
        {7, 0, 3},
        {6, 5, 4}
    };
    int from_abs = absDirMatrix[dySign + 1][dxSign + 1];
    static const int bcastDir[4][8] = {
        {1, 8, 7, 6, 5, 4, 3, 2},
        {3, 2, 1, 8, 7, 6, 5, 4},
        {5, 4, 3, 2, 1, 8, 7, 6},
        {7, 6, 5, 4, 3, 2, 1, 8}
    };
    return bcastDir[receiverDir - 1][from_abs - 1];
}

static const std::array<Vector2D, static_cast<int>(Direction::MAX)> directions = {{
    {0, 0}, {0, -1}, {1, 0}, {0, 1}, {-1, 0}
}};
static const std::array<Vector2D, static_cast<int>(Direction::MAX)> look_w_directions = {{
    {0, 0}, {1, 0}, {0, 1}, {-1, 0}, {0, -1}
}};
static const std::array<std::string_view, static_cast<int>(ResourceType::MAX)> resNames = {
    "food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"
};

static const std::unordered_map<std::string, AiCommandHandler> aiDispatch = {
    { "Forward", AiCommands::forward },
    { "Right", AiCommands::right },
    { "Left", AiCommands::left },
    { "Look", AiCommands::look },
    { "Inventory", AiCommands::inventory },
    { "Connect_nbr", AiCommands::connectNbr },
    { "Fork", AiCommands::fork },
    { "Eject", AiCommands::eject },
    { "Incantation", AiCommands::incantation },
    { "Broadcast", AiCommands::broadcast },
    { "Take", AiCommands::take },
    { "Set", AiCommands::set },
};

unsigned int AiCommands::getCommandTicks(const std::string &commandName)
{
    if (commandName == "Inventory")
        return 1;
    if (commandName == "Connect_nbr")
        return 0;
    if (commandName == "Fork")
        return 42;
    if (commandName == "Incantation")
        return 300;
    return 7;
}

void AiCommands::dispatch(Client &client, Server &server, const std::string &line)
{
    std::size_t spacePos = line.find(' ');
    std::string commandName = line.substr(0, spacePos);
    std::string args = spacePos != std::string::npos ? line.substr(spacePos + 1) : "";

    auto entry = aiDispatch.find(commandName);
    if (entry != aiDispatch.end()) {
        entry->second(client, server, args);
        return;
    }
    logger.warn("Client fd " + std::to_string(client.getFd()) + " sent unknown command: " + commandName);
    server.getSocket().sendMessage(client.getFd(), "ko\n", 3);
}

void AiCommands::forward(Client &client, Server &server, const std::string &)
{
    int currentX = client.getPlayerData()->getX();
    int currentY = client.getPlayerData()->getY();
    unsigned int currentDir = client.getPlayerData()->getDirection();

    currentX += directions[currentDir].x;
    currentY += directions[currentDir].y;
    if (currentX < 0)
        currentX += server.getWidth();
    if (currentY < 0)
        currentY += server.getHeight();
    client.getPlayerData()->setX(currentX % server.getWidth());
    client.getPlayerData()->setY(currentY % server.getHeight());
    server.visitTile(currentX % server.getWidth(), currentY % server.getHeight());
    GuiCommands::ppo(server, client.getFd());
    server.getSocket().sendMessage(client.getFd(), "ok\n", 3);
}

void AiCommands::right(Client &client, Server &server, const std::string &)
{
    unsigned int currentDir = client.getPlayerData()->getDirection();

    currentDir = (currentDir % 4) + 1;
    client.getPlayerData()->setDirection(currentDir);
    GuiCommands::ppo(server, client.getFd());
    server.getSocket().sendMessage(client.getFd(), "ok\n", 3);
}

void AiCommands::left(Client &client, Server &server, const std::string &)
{
    unsigned int currentDir = client.getPlayerData()->getDirection();

    currentDir = (currentDir == 1) ? 4 : currentDir - 1;
    client.getPlayerData()->setDirection(currentDir);
    GuiCommands::ppo(server, client.getFd());
    server.getSocket().sendMessage(client.getFd(), "ok\n", 3);
}

void AiCommands::look(Client &client, Server &server, const std::string &)
{
    unsigned int currentX = client.getPlayerData()->getX();
    unsigned int currentY = client.getPlayerData()->getY();
    unsigned int currentDir = client.getPlayerData()->getDirection();
    unsigned int currentLevel = client.getPlayerData()->getLevel();
    std::string response = "[";

    bool firstTile = true;
    for (unsigned int depth = 0; depth <= currentLevel; depth++) {
        for (int widthOffset = -static_cast<int>(depth); widthOffset <= static_cast<int>(depth); widthOffset++) {
            if (!firstTile) {
                response += ",";
            } else {
                firstTile = false;
            }
            int targetX = static_cast<int>(currentX) + directions[currentDir].x * depth + look_w_directions[currentDir].x * widthOffset;
            int targetY = static_cast<int>(currentY) + directions[currentDir].y * depth + look_w_directions[currentDir].y * widthOffset;
            while (targetX < 0) {
                targetX += server.getWidth();
            }
            while (targetY < 0) {
                targetY += server.getHeight();
            }
            targetX = targetX % server.getWidth();
            targetY = targetY % server.getHeight();

            for (const Client &otherClient : server.getClients()) {
                if (!otherClient.isDead() && otherClient.getState() == ClientState::AI && otherClient.getPlayerData()->getX() == static_cast<unsigned int>(targetX) && otherClient.getPlayerData()->getY() == static_cast<unsigned int>(targetY)) {
                    response += " player";
                }
            }
            Tile &tile = server.getMap().getTile(targetX, targetY);
            for (int resourceType = 0; resourceType < 7; resourceType++) {
                for (unsigned int resourceCount = 0; resourceCount < tile.resources[resourceType]; resourceCount++) {
                    response += " ";
                    response += resNames[resourceType];
                }
            }
        }
    }
    std::string cleanedResponse;
    for (size_t charIndex = 0; charIndex < response.size(); charIndex++) {
        if (response[charIndex] == ' ' && (charIndex == 0 || response[charIndex-1] == '[' || response[charIndex-1] == ',')) {
            continue;
        }
        cleanedResponse += response[charIndex];
    }
    cleanedResponse += "]\n";
    server.getSocket().sendMessage(client.getFd(), cleanedResponse.c_str(), cleanedResponse.size());
}

void AiCommands::inventory(Client &client, Server &server, const std::string &)
{
    std::string response = "[food " + std::to_string(client.getPlayerData()->getInventory(0)) + ", linemate " + std::to_string(client.getPlayerData()->getInventory(1)) +
        ", deraumere " + std::to_string(client.getPlayerData()->getInventory(2)) + ", sibur " + std::to_string(client.getPlayerData()->getInventory(3)) +
        ", mendiane " + std::to_string(client.getPlayerData()->getInventory(4)) + ", phiras " + std::to_string(client.getPlayerData()->getInventory(5)) +
        ", thystame " + std::to_string(client.getPlayerData()->getInventory(6)) + "]\n";
    server.getSocket().sendMessage(client.getFd(), response.c_str(), response.size());
}

void AiCommands::broadcast(Client &client, Server &server, const std::string &args)
{
    std::string text = args;
    if (!text.empty() && text.back() == '\n') {
        text.pop_back();
    }

    logger.write("A voice echoes through the plains: \"" + text + "\"");

    for (Client &receiver : server.getClients()) {
        if (&receiver != &client && !receiver.isDead() && receiver.getState() == ClientState::AI) {
            int soundDirection = getSoundDirection(
                client.getPlayerData()->getX(), client.getPlayerData()->getY(),
                receiver.getPlayerData()->getX(), receiver.getPlayerData()->getY(),
                server.getWidth(), server.getHeight(),
                receiver.getPlayerData()->getDirection()
            );
            std::string msg = "message " + std::to_string(soundDirection) + ", " + text + "\n";
            server.getSocket().sendMessage(receiver.getFd(), msg.c_str(), msg.size());
        }
    }
    GuiCommands::pbc(server, client.getFd(), text);
    server.getSocket().sendMessage(client.getFd(), "ok\n", 3);
}

void AiCommands::connectNbr(Client &client, Server &server, const std::string &)
{
    unsigned int aliveCount = 0;
    for (const Client &otherClient : server.getClients()) {
        if (!otherClient.isDead() && otherClient.getState() == ClientState::AI && otherClient.getTeamName() == client.getTeamName()) {
            aliveCount++;
        }
    }
    unsigned int totalSlots = server.getClientsNb();
    for (const Egg &egg : server.getEggs()) {
        if (egg.teamName == client.getTeamName())
            totalSlots++;
    }
    int remaining = (int)totalSlots - (int)aliveCount;
    if (remaining < 0)
        remaining = 0;
    std::string response = std::to_string(remaining) + "\n";
    server.getSocket().sendMessage(client.getFd(), response.c_str(), response.size());
}

void AiCommands::fork(Client &client, Server &server, const std::string &)
{
    GuiCommands::pfk(server, client.getFd());
    unsigned int id = server.addEgg(client.getTeamName(), client.getPlayerData()->getX(), client.getPlayerData()->getY());
    GuiCommands::enw(server, id, client.getFd(), client.getPlayerData()->getX(), client.getPlayerData()->getY());
    logger.write("An egg has been laid on the ground by team " + client.getTeamName() + ".");
    server.getSocket().sendMessage(client.getFd(), "ok\n", 3);
}

void AiCommands::eject(Client &client, Server &server, const std::string &)
{
    bool ejected = false;
    unsigned int pusherX = client.getPlayerData()->getX();
    unsigned int pusherY = client.getPlayerData()->getY();
    unsigned int pusherDir = client.getPlayerData()->getDirection();

    static const int k_matrix[4][4] = {
        {1, 7, 5, 3},
        {3, 1, 7, 5},
        {5, 3, 1, 7},
        {7, 5, 3, 1}
    };
    static const int opposite_dir[] = {0, 3, 4, 1, 2};
    unsigned int from_dir_abs = opposite_dir[pusherDir];

    for (Client &targetClient : server.getClients()) {
        if (&targetClient != &client && !targetClient.isDead() && targetClient.getState() == ClientState::AI
            && targetClient.getPlayerData()->getX() == pusherX && targetClient.getPlayerData()->getY() == pusherY) {
            ejected = true;
            int nextX = pusherX + directions[pusherDir].x;
            int nextY = pusherY + directions[pusherDir].y;
            if (nextX < 0)
                nextX += server.getWidth();
            if (nextY < 0)
                nextY += server.getHeight();
            targetClient.getPlayerData()->setX(nextX % server.getWidth());
            targetClient.getPlayerData()->setY(nextY % server.getHeight());
            int ejectDirection = k_matrix[targetClient.getPlayerData()->getDirection() - 1][from_dir_abs - 1];
            std::string msg = "eject: " + std::to_string(ejectDirection) + "\n";
            server.getSocket().sendMessage(targetClient.getFd(), msg.c_str(), msg.size());
            GuiCommands::ppo(server, targetClient.getFd());
        }
    }
    server.destroyEggsOnTile(pusherX, pusherY);
    if (ejected) {
        GuiCommands::pex(server, client.getFd());
        server.getSocket().sendMessage(client.getFd(), "ok\n", 3);
    } else
        server.getSocket().sendMessage(client.getFd(), "ko\n", 3);
}

void AiCommands::take(Client &client, Server &server, const std::string &object)
{
    int resourceIndex = -1;
    for (int currentResIndex = 0; currentResIndex < static_cast<int>(ResourceType::MAX); currentResIndex++) {
        if (object == resNames[currentResIndex]) {
            resourceIndex = currentResIndex;
            break;
        }
    }
    if (resourceIndex == -1) {
        server.getSocket().sendMessage(client.getFd(), "ko\n", 3);
        return;
    }
    Tile &tile = server.getMap().getTile(client.getPlayerData()->getX(), client.getPlayerData()->getY());
    if (tile.resources[resourceIndex] > 0) {
        tile.resources[resourceIndex]--;
        client.getPlayerData()->setInventory(resourceIndex, client.getPlayerData()->getInventory(resourceIndex) + 1);
        GuiCommands::pgt(server, client.getFd(), resourceIndex);
        GuiCommands::pin(server, client.getFd());
        GuiCommands::bct_broadcast(server, client.getPlayerData()->getX(), client.getPlayerData()->getY());
        server.getSocket().sendMessage(client.getFd(), "ok\n", 3);
    } else {
        server.getSocket().sendMessage(client.getFd(), "ko\n", 3);
    }
}

void AiCommands::set(Client &client, Server &server, const std::string &object)
{
    int resourceIndex = -1;
    for (int currentResIndex = 0; currentResIndex < static_cast<int>(ResourceType::MAX); currentResIndex++) {
        if (object == resNames[currentResIndex]) {
            resourceIndex = currentResIndex;
            break;
        }
    }
    if (resourceIndex == -1) {
        server.getSocket().sendMessage(client.getFd(), "ko\n", 3);
        return;
    }
    if (client.getPlayerData()->getInventory(resourceIndex) > 0) {
        client.getPlayerData()->setInventory(resourceIndex, client.getPlayerData()->getInventory(resourceIndex) - 1);
        Tile &tile = server.getMap().getTile(client.getPlayerData()->getX(), client.getPlayerData()->getY());
        tile.resources[resourceIndex]++;
        GuiCommands::pdr(server, client.getFd(), resourceIndex);
        GuiCommands::pin(server, client.getFd());
        GuiCommands::bct_broadcast(server, client.getPlayerData()->getX(), client.getPlayerData()->getY());
        server.getSocket().sendMessage(client.getFd(), "ok\n", 3);
    } else {
        server.getSocket().sendMessage(client.getFd(), "ko\n", 3);
    }
}

namespace Elevation {
    bool canIncant(const Client &client, Server &server, unsigned int &requiredPlayers) {
        unsigned int currentLevel = client.getPlayerData()->getLevel();
        if (currentLevel >= 8) {
            return false;
        }

        static const unsigned int requirements[7][7] = {
            {1, 1, 0, 0, 0, 0, 0},
            {2, 1, 1, 1, 0, 0, 0},
            {2, 2, 0, 1, 0, 2, 0},
            {4, 1, 1, 2, 0, 1, 0},
            {4, 1, 2, 1, 3, 0, 0},
            {6, 1, 2, 3, 0, 1, 0},
            {6, 2, 2, 2, 2, 2, 1}
        };
        requiredPlayers = requirements[currentLevel - 1][0];
        Tile &tile = server.getMap().getTile(client.getPlayerData()->getX(), client.getPlayerData()->getY());
        for (int index = 1; index <= 6; index++) {
            if (tile.resources[index] < requirements[currentLevel - 1][index]) {
                return false;
            }
        }
        return true;
    }

    std::vector<int> gatherParticipants(const Client &initiator, Server &server) {
        std::vector<int> fds;
        for (Client &participant : server.getClients()) {
            if (!participant.isDead() && participant.getState() == ClientState::AI && participant.getPlayerData()->getX() == initiator.getPlayerData()->getX()
                && participant.getPlayerData()->getY() == initiator.getPlayerData()->getY() && participant.getPlayerData()->getLevel() == initiator.getPlayerData()->getLevel()) {
                fds.push_back(participant.getFd());
            }
        }
        return fds;
    }

    void consumeResources(const Client &client, Server &server) {
        static const unsigned int requirements[7][7] = {
            {1, 1, 0, 0, 0, 0, 0},
            {2, 1, 1, 1, 0, 0, 0},
            {2, 2, 0, 1, 0, 2, 0},
            {4, 1, 1, 2, 0, 1, 0},
            {4, 1, 2, 1, 3, 0, 0},
            {6, 1, 2, 3, 0, 1, 0},
            {6, 2, 2, 2, 2, 2, 1}
        };
        unsigned int currentLevel = client.getPlayerData()->getLevel();
        Tile &tile = server.getMap().getTile(client.getPlayerData()->getX(), client.getPlayerData()->getY());
        for (int index = 1; index <= 6; index++) {
            tile.resources[index] -= requirements[currentLevel - 1][index];
        }
    }

    void elevate(const std::vector<int> &fds, Server &server, unsigned int newLevel) {
        for (int fd : fds) {
            for (Client &participant : server.getClients()) {
                if (participant.getFd() == fd) {
                    participant.getPlayerData()->setLevel(newLevel);
                    server.getSocket().sendMessage(fd, "Elevation underway\n", 19);
                    std::string msg = "Current level: " + std::to_string(newLevel) + "\n";
                    server.getSocket().sendMessage(fd, msg.c_str(), msg.size());
                    break;
                }
            }
        }
    }
}

void AiCommands::incantation(Client &client, Server &server, const std::string &)
{
    unsigned int requiredPlayers = 0;
    if (!Elevation::canIncant(client, server, requiredPlayers)) {
        server.getSocket().sendMessage(client.getFd(), "ko\n", 3);
        return;
    }

    std::vector<int> participantFds = Elevation::gatherParticipants(client, server);
    if (participantFds.size() < requiredPlayers) {
        server.getSocket().sendMessage(client.getFd(), "ko\n", 3);
        return;
    }

    GuiCommands::pic(server, client.getPlayerData()->getX(), client.getPlayerData()->getY(), client.getPlayerData()->getLevel());

    Elevation::consumeResources(client, server);
    Elevation::elevate(participantFds, server, client.getPlayerData()->getLevel() + 1);

    logger.write("A bright flash engulfs the players! They ascended to a higher state of being.");

    GuiCommands::pie(server, client.getPlayerData()->getX(), client.getPlayerData()->getY(), true);
    for (int fd : participantFds) {
        GuiCommands::plv(server, fd);
    }
    GuiCommands::bct_broadcast(server, client.getPlayerData()->getX(), client.getPlayerData()->getY());
}

}
