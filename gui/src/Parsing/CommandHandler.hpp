/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** CommandHandler
*/

#ifndef COMMANDHANDLER_HPP_
#define COMMANDHANDLER_HPP_

#include <string>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <iostream>
#include "World.hpp"
#include "Interfaces/IGui.hpp"

class CommandHandler {
    public:
        CommandHandler(World &world, IGui &gui);
        ~CommandHandler() = default;
        void handle(const std::string &line);
    private:
        std::unordered_map<std::string, void (CommandHandler::*)(std::stringstream &)> _commands;
        World &_world;
        IGui &_gui;
        void handle_msz(std::stringstream &ss);
        void handle_bct(std::stringstream &ss);
        void handle_sgt(std::stringstream &ss);
        void handle_tna(std::stringstream &ss);
        void handle_enw(std::stringstream &ss);
        void handle_pnw(std::stringstream &ss);
        void handle_ppo(std::stringstream &ss);
        void handle_ebo(std::stringstream &ss);
        void handle_pin(std::stringstream &ss);
        void handle_pgt(std::stringstream &ss);
        void handle_pdi(std::stringstream &ss);
        void handle_pic(std::stringstream &ss);
        void handle_pie(std::stringstream &ss);
        void handle_pex(std::stringstream &ss);
        void handle_pbc(std::stringstream &ss);
        void handle_pfk(std::stringstream &ss);
        void handle_pdr(std::stringstream &ss);
        void handle_edi(std::stringstream &ss);
        void handle_sst(std::stringstream &ss);
        void handle_seg(std::stringstream &ss);
        void handle_smg(std::stringstream &ss);
        void handle_plv(std::stringstream &ss);
};
