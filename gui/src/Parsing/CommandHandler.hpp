/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** CommandHandler
*/

#pragma once

#include <string>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <iostream>
#include "Sfml.hpp"

class CommandHandler {
    public:
        CommandHandler(Sfml &gui);
        ~CommandHandler() = default;
        void handle(const std::string &line);
    private:
        std::unordered_map<std::string, void (CommandHandler::*)(std::stringstream &)> _commands;
        Sfml &_gui;
        void handle_msz(std::stringstream &ss);
        void handle_bct(std::stringstream &ss);
        void handle_sgt(std::stringstream &ss);
};