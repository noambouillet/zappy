/*
** EPITECH PROJECT, 2026
** ServerData
** File description:
** Command line parsing and server configuration storage
*/

#include "ServerData.hpp"
#include "GameLoop/Server.hpp"
#include "ServerExceptions.hpp"
#include <cstdlib>
#include <array>
#include <iostream>

namespace ZappyServer {

static unsigned int parsePositiveInt(const std::string &value, const std::string &label)
{
    char *end = nullptr;
    const long parsed = std::strtol(value.c_str(), &end, 10);

    if (value.empty() || end == nullptr || *end != '\0' || parsed <= 0)
        throw ServerException("Invalid " + label + ": '" + value + "'.");
    return static_cast<unsigned int>(parsed);
}

static unsigned int parseBoundedInt(const std::string &value, const std::string &label, unsigned int min, unsigned int max)
{
    const unsigned int parsed = parsePositiveInt(value, label);

    if (parsed < min || parsed > max)
        throw ServerException("Invalid " + label + ": must be between " + std::to_string(min) + " and " + std::to_string(max) + ".");
    return parsed;
}

static bool isKnownOption(const std::string &value)
{
    std::array<std::string, 7> options = {"-p", "-x", "-y", "-n", "-c", "-f", "--help"};

    for (std::string &option : options) {
        if (option == value)
            return true;
    }
    return false;
}

static std::string requireValue(int argc, char **argv, int &index, const std::string &option)
{
    if (index + 1 >= argc)
        throw ServerException("Unknown or incomplete argument: " + option + ".");
    index++;
    return argv[index];
}

static void parseTeamNames(std::vector<std::string> &teamNames, int argc, char **argv, int &index)
{
    const std::size_t initialCount = teamNames.size();

    index++;
    while (index < argc && !isKnownOption(argv[index])) {
        const std::string teamName = argv[index];

        if (teamName == "GRAPHIC")
            throw ServerException("Team name GRAPHIC is reserved.");
        for (const std::string &existingName : teamNames) {
            if (existingName == teamName)
                throw ServerException("Duplicate team name: " + teamName + ".");
        }
        teamNames.push_back(teamName);
        index++;
    }
    index--;
    if (teamNames.size() == initialCount)
        throw ServerException("Missing team names after -n.");
}

void ServerData::parseArgs(char **argv, int argc, int &index)
{
    const std::string option = argv[index];

    if (option == "-p") {
        _port = parseBoundedInt(requireValue(argc, argv, index, option), "port", 1024, 65535);
        return;
    }
    if (option == "-x") {
        _width = parseBoundedInt(requireValue(argc, argv, index, option), "width", 10, 42);
        return;
    }
    if (option == "-y") {
        _height = parseBoundedInt(requireValue(argc, argv, index, option), "height", 10, 42);
        return;
    }
    if (option == "-n") {
        parseTeamNames(_teamNames, argc, argv, index);
        return;
    }
    if (option == "-c") {
        _clientsNb = parseBoundedInt(requireValue(argc, argv, index, option), "clients number", 1, 200);
        return;
    }
    if (option == "-f") {
        _freq = parseBoundedInt(requireValue(argc, argv, index, option), "freq", 1, 10000);
        return;
    }
    throw ServerException("Unknown or incomplete argument: " + option + ".");
}

void ServerData::parse(int argc, char **argv)
{
    if (argc == 2 && std::string(argv[1]) == "--help") {
        printHelp(argv[0]);
        std::exit(0);
    }
    for (int index = 1; index < argc; index++)
        parseArgs(argv, argc, index);
    if (_port == 0 || _width == 0 || _height == 0 || _clientsNb == 0 || _teamNames.empty())
        throw ServerException("Missing required arguments. Use --help for usage.");
}

void ServerData::printHelp(const std::string &binaryName) const
{
    std::cout << "USAGE: " << binaryName << " -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq\n"
        << "\tport\t\t is the port number\n" << "\twidth\t\t is the width of the world\n" << "\theight\t\t is the height of the world\n"
        << "\tnameZ\t\t is the name of the team Z\n" << "\tclientsNb\t is the number of authorized clients per team\n"
        << "\tfreq\t\t is the reciprocal of time unit for execution of actions\n";
}

ServerData::ServerData() : _port(0), _width(0), _height(0), _clientsNb(0), _freq(100)
{
}

void ServerData::run() const
{
    Server server(_port, _width, _height, _clientsNb, _freq, _teamNames);
    server.run();
}

unsigned int ServerData::getPort() const
{
    return _port;
}

unsigned int ServerData::getWidth() const
{
    return _width;
}

unsigned int ServerData::getHeight() const
{
    return _height;
}

unsigned int ServerData::getClientsNb() const
{
    return _clientsNb;
}

unsigned int ServerData::getFreq() const
{
    return _freq;
}

const std::vector<std::string> &ServerData::getTeamNames() const
{
    return _teamNames;
}

}
