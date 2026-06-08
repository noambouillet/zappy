/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** CommandHandler
*/

#include "CommandHandler.hpp"

CommandHandler::CommandHandler(Sfml &gui) : _gui(gui)
{
    _commands["msz"] = &CommandHandler::handle_msz;
    _commands["bct"] = &CommandHandler::handle_bct;
    _commands["sgt"] = &CommandHandler::handle_sgt;
}

void CommandHandler::handle(const std::string &line)
{
    if (line.empty())
        return;

    std::stringstream ss(line);
    std::string cmd;
    
    ss >> cmd;
    auto it = _commands.find(cmd);
    if (it != _commands.end()) {
        (this->*(it->second))(ss);
    } else {
        std::cout << "Unknow command : " << cmd << std::endl;
    }
}

void CommandHandler::handle_msz(std::stringstream &ss)
{
    int width, height;

    if (!(ss >> width >> height))
        return;
    _gui.setMapSize(width, height);
}

void CommandHandler::handle_bct(std::stringstream &ss)
{
    int x, y, q0, q1, q2, q3, q4, q5, q6;

    if (!(ss >> x >> y >> q0 >> q1 >> q2 >> q3 >> q4 >> q5 >> q6))
        return;
    _gui.setTile(x, y, {q0, q1, q2, q3, q4, q5, q6});
}

void CommandHandler::handle_sgt(std::stringstream &ss)
{
    int time_unit;

    if (!(ss >> time_unit))
        return;
    _gui.setTimeUnit(time_unit);
}