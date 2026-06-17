/*
** EPITECH PROJECT, 2026
** Plazza
** File description:
** InputParser
*/

#include "InputParser.hpp"
#include "ServerExceptions.hpp"
#include "Server.hpp"
#include "GuiCommands.hpp"
#include <cstdlib>

namespace ZappyServer {

InputParser::InputParser(Server& server) : _server(server) {}

static void printHelpShell()
{
    std::cout << "COMMANDS:\n" << "\t/help\t\t gives you the list of the shell's commands\n"
        << "\t/exit\t\t lets you leave the shell\n" << "\t/clear\t\t lets you clear the screen.\n"
        << "\t/pause\t\t stops the gameloop and doesn't compute ticks.\n" << "\t/resume\t\t resumes the gameloop.\n"
        << "\t/setFreq <freq>\t changes the frequency.\n" << "\t/clients\t shows connected clients.\n"
        << "\t/map\t\t shows the map.\n" << "\t/kill <fd>\t kills an AI client.\n";
}

static unsigned int parsePositiveInt(const std::string &value, const std::string &label)
{
    char *end = nullptr;
    const long parsed = std::strtol(value.c_str(), &end, 10);

    if (value.empty() || end == nullptr || *end != '\0' || parsed <= 0) {
        printHelpShell();
        throw ServerException("Invalid " + label + ": '" + value + "'. Must be a positive integer.");
    }
    return static_cast<unsigned int>(parsed);
}

static void broadcastSst(Server &server)
{
    std::string msg = "sst " + std::to_string(server.getFreq()) + "\n";

    for (Client &client : server.getClients()) {
        if (!client.isDead() && client.getState() == ClientState::GUI)
            server.getSocket().sendMessage(client.getFd(), msg.c_str(), msg.size());
    }
}

void InputParser::printClients() const
{
    for (const Client &client : _server.getClients()) {
        if (client.isDead())
            continue;
        std::cout << "================USER================\n";
        std::cout << "fd = " << client.getFd() << '\n';
        std::cout << "Type = ";
        switch (client.getState()) {
            case ClientState::AI:
                std::cout << "AI";
                break;
            case ClientState::GUI:
                std::cout << "GUI";
                break;
            default:
                std::cout << "WAITING_TEAM";
                break;
        }
        if (!client.getTeamName().empty())
            std::cout << "\nTeam = " << client.getTeamName();
        if (client.getState() == ClientState::AI && client.getPlayerData().has_value()) {
            const PlayerData &player = client.getPlayerData().value();
            std::cout << "\nPosition = (" << player.getX() << ", " << player.getY() << ")";
            std::cout << "\nLevel = " << player.getLevel();
        }
        std::cout << '\n';
        std::cout << "====================================\n";
    }
}

void InputParser::printMap() const
{
    unsigned int height = _server.getHeight();
    unsigned int width = _server.getWidth();
    Map map = _server.getMap();

    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            const Tile &tile = map.getTile(x, y);
            std::cout << "(" << x << "," << y << ") : ";
            for (unsigned int i = 0; i < RESOURCES_NUMBER; i++)
                std::cout << tile.resources[i] << ' ';
            std::cout << '\n';
        }
        if (y != height - 1)
            std::cout << '\n';
    }
}

void InputParser::processCommand(const std::string& command)
{
    if (command.empty())
        return;
    if (command == "/exit") {
        _server.stop();
    } else if (command == "/help") {
        printHelpShell();
    } else if (command == "/clear") {
        std::cout << "\033[2J\033[H";
    } else if (command == "/pause") {
        _server.pause();
    } else if (command == "/resume") {
        _server.resume();
    } else if (command.rfind("/setFreq ", 0) == 0) {
        unsigned int freq = parsePositiveInt(command.substr(9), "freq");
        _server.setFreq(freq);
        broadcastSst(_server);
    } else if (command.rfind("/kill ", 0) == 0) {
        unsigned int fd = parsePositiveInt(command.substr(6), "fd");
        _server.killClient(fd);
    } else if (command == "/clients") {
        printClients();
    } else if (command == "/map") {
        printMap();
    } else {
        throw ServerException("Invalid command.");
    }
}

}
