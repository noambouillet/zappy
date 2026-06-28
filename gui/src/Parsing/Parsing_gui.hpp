/*
** EPITECH PROJECT, 2026
** G-NWP-400-LIL-4-1-myteams-8
** File description:
** Parsing_gui
*/

#ifndef PARSING_GUI_HPP_
#define PARSING_GUI_HPP_

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef struct networkData_s {
    int port;
    std::string ip;
    bool use3D;
} networkData_t;

class Parsing_gui
{
    public:
        Parsing_gui();
        networkData_t parse_args(int ac, char **av);
        void isTTY(char **env) const;
        void print_help();
        networkData_t check_args(const std::string &addr, const std::string &port);
        bool is_ipv4(const std::string &addr);
};

#endif /* !PARSING_GUI_HPP_ */
