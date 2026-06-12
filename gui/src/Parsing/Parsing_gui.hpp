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

typedef struct networkData_s {
    int port;
    std::string ip;
} networkData_t;

class Parsing_gui
{
    public:
        Parsing_gui();
        networkData_t parse_args(int ac, char **av);
        void print_help();
        networkData_t check_args(char *addr, char *port);
        bool is_ipv4(char *addr);
        class ARGS_ERROR : public std::exception {
            public:
                ARGS_ERROR(std::string error);
                const char *what() const noexcept override;
            private:
                std::string _msg;
        };
};
