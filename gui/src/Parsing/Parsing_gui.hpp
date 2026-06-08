/*
** EPITECH PROJECT, 2026
** G-NWP-400-LIL-4-1-myteams-8
** File description:
** Parsing_gui
*/

#pragma once

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class Parsing_gui
{
private:
    int _port;
    std::string _ip;
public:
    Parsing_gui();
    int parse_args(int ac, char **av);
    void print_help();
    void check_args(char *addr, char *port);
    bool is_ipv4(char *addr);
    int connect_to_server();
    void send_command(int fd, const std::string& msg);
    std::string read_from_server(int fd);
    bool extract_message(std::string &buffer, std::string &message);
    class ARGS_ERROR : public std::exception {
        public:
            ARGS_ERROR(std::string error);
            const char *what() const noexcept override;
        private:
            std::string _msg;
    };
};
