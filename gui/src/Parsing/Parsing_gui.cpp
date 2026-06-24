/*
** EPITECH PROJECT, 2026
** G-NWP-400-LIL-4-1-myteams-8
** File description:
** Parsing_gui
*/

#include "Parsing_gui.hpp"
#include <vector>
#include <string>
#include "GuiExceptions.hpp"
#include "Logger.hpp"

Parsing_gui::Parsing_gui()
{
}

void Parsing_gui::print_help()
{
    logger.info("USAGE: ./zappy_gui -p port -h machine [-3d]");
}

bool Parsing_gui::is_ipv4(const std::string &addr)
{
    int a;
    int b;
    int c;
    int d;
    char end;

    if (addr.empty() || addr[0] == '\0' || sscanf(addr.c_str(), "%3d.%3d.%3d.%3d%c", &a, &b, &c, &d, &end) != 4 || (a < 0 || a > 255) || (b < 0 || b > 255) || (c < 0 || c > 255) || (d < 0 || d > 255))
        return false;
    return true;
}

networkData_t Parsing_gui::check_args(const std::string &addr, const std::string &port)
{
    networkData_t data;
    if (addr == "localhost")
        data.ip = "127.0.0.1";
    else {
        if (!is_ipv4(addr))
            throw ParsingGuiException("The address: '" + addr + "' does not feat the ipv4 format");
        data.ip = addr;
    }

    for (std::size_t i = 0; i < port.length(); i++)
        if (port[i] < '0' || port[i] > '9')
            throw ParsingGuiException("The port: '" + port + "' is not a number");
    data.port = std::stoi(port);
    if (data.port < 1 || data.port > 65535)
        throw ParsingGuiException("The port: '" + port + "' is out of range (port must be between 1 and 65535)");

    return data;
}

networkData_t Parsing_gui::parse_args(int ac, char **av)
{
    if (ac == 2 && std::string(av[1]) == "--help") {
        print_help();
        exit(0);
    }
    std::string port = "";
    std::string addr = "";
    bool use3D = false;
    for (int i = 1; i < ac; i++) {
        std::string flag = av[i];
        if (flag == "-3d") {
            use3D = true;
            continue;
        }
        if (flag == "-v" || flag == "--verbose") {
            logger.setVerbose(true);
            continue;
        }
        if (i + 1 >= ac) {
            throw ParsingGuiException("Missing value for flag " + flag);
        }
        if (flag == "-p") {
            port = av[i + 1];
            i++;
        } else if (flag == "-h") {
            addr = av[i + 1];
            i++;
        } else {
            throw ParsingGuiException("Unknown flag: " + flag);
        }
    }
    if (port.empty()) {
        throw ParsingGuiException("Missing mandatory flag: -p <port>");
    }
    if (addr.empty()) {
        throw ParsingGuiException("Missing mandatory flag: -h <machine>");
    }
    networkData_t data = check_args(addr, port);
    data.use3D = use3D;
    return data;
}