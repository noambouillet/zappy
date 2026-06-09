/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** CommandHandler
*/

#include "CommandHandler.hpp"

CommandHandler::CommandHandler(World &world) : _world(world)
{
    _commands["msz"] = &CommandHandler::handle_msz;
    _commands["bct"] = &CommandHandler::handle_bct;
    _commands["sgt"] = &CommandHandler::handle_sgt;
    _commands["tna"] = &CommandHandler::handle_tna;
    _commands["enw"] = &CommandHandler::handle_enw;
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
        std::cout << "Unknow command : " << line << std::endl;
    }
}

void CommandHandler::handle_msz(std::stringstream &ss)
{
    int width, height;

    if (!(ss >> width >> height))
        return;
    _world.setMapSize(width, height);
}

void CommandHandler::handle_bct(std::stringstream &ss)
{
    int x, y, q0, q1, q2, q3, q4, q5, q6;

    if (!(ss >> x >> y >> q0 >> q1 >> q2 >> q3 >> q4 >> q5 >> q6))
        return;
    _world.setTile(x, y, {q0, q1, q2, q3, q4, q5, q6});
}

void CommandHandler::handle_sgt(std::stringstream &ss)
{
    int time_unit;

    if (!(ss >> time_unit))
        return;
    _world.setTimeUnit(time_unit);
}

void CommandHandler::handle_tna(std::stringstream &ss)
{
    std::string teamName;

    if (!(ss >> teamName))
        return;
    _world.addTeam(teamName);
}

void CommandHandler::handle_enw(std::stringstream &ss)
{
    int eggNb, playerNb, x, y;
    char sharp1, sharp2;
    
    if (!(ss >> sharp1 >> eggNb >> sharp2 >> playerNb >> x >> y))
        return;
    _world.addEgg({eggNb, playerNb, x, y});
}

void CommandHandler::handle_pnw(std::stringstream &ss)
{
    Player_t player;

    if (!(ss >> player.id >> player.x >> player.y >> player.orientation >> player.level >> player.teamName))
        return;
    _world.addPlayer(player);
}
