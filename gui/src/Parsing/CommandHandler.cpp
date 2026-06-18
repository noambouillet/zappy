/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** CommandHandler
*/

#include "CommandHandler.hpp"
#include "Logger.hpp"

CommandHandler::CommandHandler(World &world) : _world(world)
{
    _commands["msz"] = &CommandHandler::handle_msz;
    _commands["bct"] = &CommandHandler::handle_bct;
    _commands["sgt"] = &CommandHandler::handle_sgt;
    _commands["tna"] = &CommandHandler::handle_tna;
    _commands["enw"] = &CommandHandler::handle_enw;
    _commands["pnw"] = &CommandHandler::handle_pnw;
    _commands["ppo"] = &CommandHandler::handle_ppo;
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
        logger.warn("Unknown command : " + line);
    }
}

void CommandHandler::handle_msz(std::stringstream &ss)
{
    int width;
    int height;

    if (!(ss >> width >> height))
        return;
    _world.setMapSize(width, height);
}

void CommandHandler::handle_bct(std::stringstream &ss)
{
    int x;
    int y;
    int q0;
    int q1;
    int q2;
    int q3;
    int q4;
    int q5;
    int q6;

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
    int eggNb;
    int playerNb;
    int  x;
    int  y;
    char sharp1;
    char sharp2;
    
    if (!(ss >> sharp1 >> eggNb >> sharp2 >> playerNb >> x >> y))
        return;
    _world.addEgg({eggNb, playerNb, x, y});
}

void CommandHandler::handle_pnw(std::stringstream &ss)
{
    Player_t player;
    char sharp;

    if (!(ss >> sharp >> player.id >> player.x >> player.y >> player.orientation >> player.level >> player.teamName))
        return;
    _world.addPlayer(player);
}

void CommandHandler::handle_ppo(std::stringstream &ss)
{
    int id;
    int x;
    int y;
    int orientation;
    char sharp;

    if (!(ss >> sharp >> id >> x >> y >> orientation))
        return;
    _world.movePlayer(id, x, y, orientation);
}
