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
#include "World.hpp"

class CommandHandler {
    public:
        CommandHandler(World &world);
        ~CommandHandler() = default;
        void handle(const std::string &line);
    private:
        std::unordered_map<std::string, void (CommandHandler::*)(std::stringstream &)> _commands;
        World &_world;
        void handle_msz(std::stringstream &ss);
        void handle_bct(std::stringstream &ss);
        void handle_sgt(std::stringstream &ss);
        void handle_tna(std::stringstream &ss);
        void handle_enw(std::stringstream &ss);
        void handle_pnw(std::stringstream &ss);
        void handle_ppo(std::stringstream &ss);
        void handle_ebo(std::stringstream &ss);
};