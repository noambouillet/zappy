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
#include "Logger.hpp"

namespace ZappyServer {

static constexpr std::string_view ascii67 = R"(    _
^^^^( )
|      |   _
|     /   ( )^^^^
|    (    |      |
|     )    \     |
\___/      )    |
         (     |
          \___/
)";

static void printHelp()
{
    std::cout
        << "USAGE: ./zappy_server [options]\n"
        << "\n"
        << "OPTIONS:\n"
        << "  -p, --port <port>\t\tServer port (1024-65535)\n"
        << "  -x, --width <width>\t\tWorld width (10-42)\n"
        << "  -y, --height <height>\t\tWorld height (10-42)\n"
        << "  -n, --teams <names...>\tTeam names (at least one required)\n"
        << "  -c, --clientnbr <nb>\t\tMax clients per team (1-200)\n"
        << "  -f, --freq <freq>\t\tTime unit frequency (1-10000)\n"
        << "  -s, --seed <seed>\t\tRandom seed (optional, strict positive integer)\n"
        << "  --help\t\t\tDisplay this help message\n";
}

static unsigned int parsePositiveInt(const std::string &value, const std::string &label)
{
    char *end = nullptr;
    const long parsed = std::strtol(value.c_str(), &end, 10);

    if (value.empty() || end == nullptr || *end != '\0' || parsed <= 0) {
        printHelp();
        throw ParsingException("Invalid " + label + ": '" + value + "'. Must be a positive integer.");
    }
    return static_cast<unsigned int>(parsed);
}

static unsigned int parseBoundedInt(const std::string &value, const std::string &label, unsigned int min, unsigned int max)
{
    const unsigned int parsed = parsePositiveInt(value, label);

    if (parsed < min || parsed > max) {
        printHelp();
        throw ParsingException("Invalid " + label + ": must be between " + std::to_string(min) + " and " + std::to_string(max) + ".");
    }
    return parsed;
}

static bool isKnownOption(const std::string &value)
{
    const std::array<std::string, 17> options = {
        "-p", "--port", "-x", "--width", "-y", "--height", "-n", "--teams", "-c",
        "--clientnbr", "-f", "--freq", "-s", "--seed", "--help", "-v", "--verbose"
    };

    for (const std::string &option : options) {
        if (option == value) {
            return true;
        }
    }
    return false;
}

static std::string requireValue(int argc, char **argv, int &index, const std::string &option)
{
    if (index + 1 >= argc) {
        throw ParsingException("Unknown or incomplete argument: " + option + ".");
    }
    index++;
    return argv[index];
}

static void parseTeamNames(std::vector<std::string> &teamNames, int argc, char **argv, int &index)
{
    const std::size_t initialCount = teamNames.size();

    index++;
    while (index < argc && !isKnownOption(argv[index])) {
        const std::string teamName = argv[index];
        if (teamName.empty()) {
            throw ParsingException("Team name cannot be empty.");
        }
        if (teamName == "GRAPHIC") {
            throw ParsingException("Team name GRAPHIC is reserved.");
        }
        for (const std::string &existingName : teamNames) {
            if (existingName == teamName) {
                throw ParsingException("Duplicate team name: " + teamName + ".");
            }
        }
        teamNames.push_back(teamName);
        index++;
    }
    index--;
    if (teamNames.size() == initialCount) {
        throw ParsingException("Missing team names after -n.");
    }
}

void ServerData::parseArgs(char **argv, int argc, int &index)
{
    const std::string option = argv[index];

    if (option == "-p" || option == "--port") {
        _port = parseBoundedInt(requireValue(argc, argv, index, option), "port", 1024, 65535);
        return;
    }
    if (option == "-x" || option == "--width") {
        _width = parseBoundedInt(requireValue(argc, argv, index, option), "width", 10, 42);
        return;
    }
    if (option == "-y" || option == "--height") {
        _height = parseBoundedInt(requireValue(argc, argv, index, option), "height", 10, 42);
        return;
    }
    if (option == "-n" || option == "--teams") {
        parseTeamNames(_teamNames, argc, argv, index);
        return;
    }
    if (option == "-c" || option == "--clientnbr") {
        _clientsNb = parseBoundedInt(requireValue(argc, argv, index, option), "clients number", 1, 200);
        return;
    }
    if (option == "-f" || option == "--freq") {
        _freq = parseBoundedInt(requireValue(argc, argv, index, option), "freq", 1, 10000);
        return;
    }
    if (option == "-s" || option == "--seed") {
        _seed = parsePositiveInt(requireValue(argc, argv, index, option), "seed");
        return;
    }
    if (option == "-v" || option == "--verbose") {
        _verbose = true;
        return;
    }
    throw ParsingException("Unknown or incomplete argument: " + option + ".");
}

void ServerData::parse(int argc, char **argv)
{
    if (argc == 2 && std::string(argv[1]) == "--help") {
        printHelp();
        std::exit(0);
    }
    for (int index = 1; index < argc; index++) {
        parseArgs(argv, argc, index);
    }
    if (_teamNames.empty()) {
        printHelp();
        throw ParsingException("Missing required arguments. Team names must be provided with -n.");
    }
}

void ServerData::printServerData() const
{
    std::cout << "------------------\n";
    std::cout << ascii67;
    std::cout << "------------------\n";
    std::cout << "port = " << _port << "\nwidth = " << _width << "\nheight = " << _height << "\nclients_nb = "
    << _clientsNb << "\nfreq = " << _freq << "\nseed = " << _seed << std::endl;
    std::cout << "------------------\n";
    std::cout << "Teams:\n";
    for (std::string team : _teamNames) {
        std::cout << "- " << team << "\n";
    }
    std::cout << "------------------\n";
}

ServerData::ServerData() : _port(4242), _width(10), _height(10), _clientsNb(3), _freq(100), _seed(0), _verbose(false)
{
}

void ServerData::run()
{
    if (_verbose) {
        logger.setVerbose(true);
    }
    if (_seed == 0)
        _seed = std::random_device{}();
    printServerData();
    Server server(_port, _width, _height, _clientsNb, _freq, _seed, _teamNames);
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

unsigned int ServerData::getSeed() const
{
    return _seed;
}

const std::vector<std::string> &ServerData::getTeamNames() const
{
    return _teamNames;
}

}
