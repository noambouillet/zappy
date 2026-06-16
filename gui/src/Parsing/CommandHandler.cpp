/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** CommandHandler
*/

#include "CommandHandler.hpp"

CommandHandler::CommandHandler(World &world, Sfml &gui) : _world(world), _handleTrantorians(gui.getHandleTrantorians())
{
    _commands["msz"] = &CommandHandler::handle_msz;
    _commands["bct"] = &CommandHandler::handle_bct;
    _commands["sgt"] = &CommandHandler::handle_sgt;
    _commands["tna"] = &CommandHandler::handle_tna;
    _commands["enw"] = &CommandHandler::handle_enw;
    _commands["pnw"] = &CommandHandler::handle_pnw;
    _commands["ppo"] = &CommandHandler::handle_ppo;
    _commands["ebo"] = &CommandHandler::handle_ebo;
    _commands["pin"] = &CommandHandler::handle_pin;
    _commands["pgt"] = &CommandHandler::handle_pgt;
    _commands["pdi"] = &CommandHandler::handle_pdi;
    _commands["pic"] = &CommandHandler::handle_pic;
    _commands["pie"] = &CommandHandler::handle_pie;
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
        std::cout << "Unknown command : " << line << std::endl;
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
    _world.addEgg(eggNb, playerNb, x, y);
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

void CommandHandler::handle_ebo(std::stringstream &ss)
{
    int eggNB;
    char sharp;

    if (!(ss >> sharp >> eggNB))
        return;
    _world.removeEgg(eggNB);
}

void CommandHandler::handle_pin(std::stringstream &ss)
{
    int id;
    char sharp;
    if (!(ss >> sharp >> id))
        return;
    _handleTrantorians.setPlayerActionBubble(id, "inventory", 1.0f / _world.getTime()); 
}

void CommandHandler::handle_pgt(std::stringstream &ss)
{
    int id;
    int resourceId;
    char sharp;

    if (!(ss >> sharp >> id >> resourceId))
        return;
    static const std::string resourceTextures[] = {
        "donut",
        "linemate",
        "emeraude",
        "rubis",
        "diamond",
        "saphir",
        "amethyst"
    };
    _handleTrantorians.setPlayerActionBubble(id, resourceTextures[resourceId], 7.0f / _world.getTime());
}

void CommandHandler::handle_pdi(std::stringstream &ss)
{
    int id;
    char sharp;

    if (!(ss >> sharp >> id))
        return;
    _handleTrantorians.triggerPlayerDeath(id);
}

void CommandHandler::handle_pic(std::stringstream &ss)
{
    int x, y, level, id;
    
    if (!(ss >> x >> y >> level))
        return;
    while (ss >> id) {
        _handleTrantorians.setPlayerIncanting(id, true);
    }
}

void CommandHandler::handle_pie(std::stringstream &ss)
{
    int x, y, result;

    if (!(ss >> x >> y >> result))
        return;
    _handleTrantorians.stopIncantationAt(x, y);
}