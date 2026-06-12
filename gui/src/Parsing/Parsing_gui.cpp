/*
** EPITECH PROJECT, 2026
** G-NWP-400-LIL-4-1-myteams-8
** File description:
** Parsing_gui
*/

#include "Parsing_gui.hpp"
#include <vector>
#include <string>
#include <sstream>

Parsing_gui::Parsing_gui()
{
}

Parsing_gui::ARGS_ERROR::ARGS_ERROR(std::string error) : _msg(error)
{
}

const char *Parsing_gui::ARGS_ERROR::what() const noexcept
{
    return _msg.c_str();
}


void Parsing_gui::print_help()
{
    std::cout << "USAGE: ./zappy_gui -p port -h machine" << std::endl;
}

bool Parsing_gui::is_ipv4(char* addr)
{
    int a;
    int b;
    int c;
    int d;
    char end;

    if (!addr || addr[0] == '\0')
        return false;
    if (sscanf(addr, "%3d.%3d.%3d.%3d%c", &a, &b, &c, &d, &end) != 4)
        return false;
    if (a < 0 || a > 255)
        return false;
    if (b < 0 || b > 255)
        return false;
    if (c < 0 || c > 255)
        return false;
    if (d < 0 || d > 255)
        return false;
    return true;
}


networkData_t Parsing_gui::check_args(char *addr, char *port)
{
    networkData_t data;
    if (!is_ipv4(addr))
        throw ARGS_ERROR("The address: '" + std::string(addr) + "' does not feat the ipv4 format");
    for (std::size_t i = 0; port[i] != '\0'; i++)
        if (port[i] < '0' || port[i] > '9')
            throw ARGS_ERROR("The port: '" + std::string(port) + "' is not a number");
    data.port = std::stoi(port);
    if (data.port < 1 || data.port > 65535)
        throw ARGS_ERROR("The port: '" + std::string(port) + "' is out of range (port must be between 1 and 65535)");
    data.ip = addr;
    return data;
}

networkData_t Parsing_gui::parse_args(int ac, char **av)
{
    if ((ac == 2 && std::string(av[1]) == "--help") || ac != 5) {
        print_help();
        throw ARGS_ERROR("Bad usage");
    }
    std::string port = "";
    std::string addr = "";
    for (int i = 1; i < ac; i += 2) {
        std::string flag = av[i];
        if (i + 1 >= ac) {
            throw ARGS_ERROR("Missing value for flag " + flag);
        }
        if (flag == "-p") {
            port = av[i + 1];
        } else if (flag == "-h") {
            addr = av[i + 1];
        } else {
            throw ARGS_ERROR("Unknown flag: " + flag);
        }
    }
    if (port.empty()) {
        throw ARGS_ERROR("Missing mandatory flag: -p <port>");
    }
    if (addr.empty()) {
        throw ARGS_ERROR("Missing mandatory flag: -h <machine>");
    }
    return check_args(const_cast<char*>(addr.c_str()), const_cast<char*>(port.c_str()));
}