/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** CommandHandler
*/

#include "CommandHandler.hpp"
#include <random>

CommandHandler::CommandHandler(World &world, IGui &gui) : _world(world), _gui(gui)
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
    _commands["pex"] = &CommandHandler::handle_pex;
    _commands["pbc"] = &CommandHandler::handle_pbc;
    _commands["pfk"] = &CommandHandler::handle_pfk;
    _commands["pdr"] = &CommandHandler::handle_pdr;
    _commands["edi"] = &CommandHandler::handle_edi;
    _commands["sst"] = &CommandHandler::handle_sst;
    _commands["seg"] = &CommandHandler::handle_seg;
    _commands["smg"] = &CommandHandler::handle_smg;
    _commands["plv"] = &CommandHandler::handle_plv;
    _commands["suc"] = &CommandHandler::handle_suc;
    _commands["sbp"] = &CommandHandler::handle_sbp;
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
    std::cout << "Map Size : " << width << "x" << height << std::endl;
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
    std::cout << "Tile " << x << ":" << y << " -> " << q0 << " " << q1 << " " << q2 << " " << q3 << " " << q4 << " " << q5 << " " << q6 << std::endl;
}

void CommandHandler::handle_sgt(std::stringstream &ss)
{
    int timeUnit;

    if (!(ss >> timeUnit))
        return;
    _world.setTimeUnit(timeUnit);
    std::cout << "Time unit : " << timeUnit << std::endl;
}

void CommandHandler::handle_tna(std::stringstream &ss)
{
    std::string teamName;

    if (!(ss >> teamName))
        return;
    _world.addTeam(teamName);
    std::cout << "Team : " << teamName << " was created." << std::endl;
}

void CommandHandler::handle_enw(std::stringstream &ss)
{
    int eggNb;
    int trantorianNb;
    int x;
    int y;
    char sharp1;
    char sharp2;
    
    if (!(ss >> sharp1 >> eggNb >> sharp2 >> trantorianNb >> x >> y))
        return;
    _world.addEgg(eggNb, trantorianNb, x, y);
    if (trantorianNb == -1)
        std::cout << "An egg has spawn." << std::endl;
    else
        std::cout << "An egg was laid by trantorian #" << trantorianNb << std::endl;
}

void CommandHandler::handle_pnw(std::stringstream &ss)
{
    Trantorian_t trantorian;
    char sharp;
    std::vector<std::string> names = {
        "trantorian le Cafouilleux", "Noam le Gâte Sauce", "Logan le Brise Assiette", "Gurtag le Mâche Pissenlit", 
        "Louis le Poltron", "Paul le Pied Bot", "Poll le Sourd-Double", "Keryan le Glandu",
        "Godefroy le Mafflu", "Aldebert le Sec", "Anselme le Putois", "Barnabe Soupe au Lait",
        "Enguerrand le Pleutre", "Foulques le Gargouilleux", "Gontran le Morveux", "Hugues le Glandu",
        "Lancelot du Lac Sacre", "Pepin le Bref de Plafond", "Thibault le Flemmard", "Eustache le Chasuble",
        "Gauvain le Maladroit", "Clovis le Baveux", "Dagobert a l Envers", "Childebert le Chauve",
        "Sigismond le Chiche", "Alphonse le Miteux", "Baudouin le Bredouille", "Clotaire le Grogneur",
        "Gondebaud le Trouillard", "Arnault le Goinfre", "Bertrand le Bistouquet", "Eudes le Sourd",
        "Guibert le Chancelant", "Raoul le Poilu", "Suger le Sirupeux", "Wenceslas le Mou",
        "Jehan le Marmite", "Gilles le Nigaud", "Renaud le Rouspéteur", "Tancrede le Brise Fer"
    };
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, names.size() - 1);

    trantorian.Name = names[distr(gen)];
    if (!(ss >> sharp >> trantorian.id >> trantorian.x >> trantorian.y >> trantorian.orientation >> trantorian.level >> trantorian.teamName))
        return;
    _world.addTrantorian(trantorian);
    std::cout << "Trantorian : " << trantorian.Name << " #" << trantorian.id << " was added." << std::endl;
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
    _world.moveTrantorian(id, x, y, orientation);
    std::cout << "Trantorian #" << id << " position -> " << x << ":" << y << std::endl;
}

void CommandHandler::handle_ebo(std::stringstream &ss)
{
    int eggNB;
    char sharp;

    if (!(ss >> sharp >> eggNB))
        return;
    _world.removeEgg(eggNB);
    std::cout << "A trantorian connect to the egg #" << eggNB << std::endl;
}

void CommandHandler::handle_pin(std::stringstream &ss)
{
    int id;
    int x;
    int y;
    int q0;
    int q1;
    int q2;
    int q3;
    int q4;
    int q5;
    int q6;
    char sharp;

    if (!(ss >> sharp >> id >> x >> y >> q0 >> q1 >> q2 >> q3 >> q4 >> q5 >> q6))
        return;
    _world.setTrantorianInventory(x, y, id, {q0, q1, q2, q3, q4, q5, q6});
    _gui.setTrantorianActionBubble(id, "inventory", 5.0f * 1.0f / _world.getTime());
    std::cout << "Trantorian #" << id << " check his inventory." << std::endl;
}

void CommandHandler::handle_pex(std::stringstream &ss)
{
    int id;
    char sharp;

    if (!(ss >> sharp >> id))
        return;
    _gui.setTrantorianActionBubble(id, "!", 5.0f * 7.0f / _world.getTime());
    std::cout << "Trantorian #" << id << " is angry and need to push things to feel better." << std::endl;
}

void CommandHandler::handle_pgt(std::stringstream &ss)
{
    int id;
    int resourceId;
    char sharp;
    static const std::string resourceTextures[] = {"donut", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};

    if (!(ss >> sharp >> id >> resourceId))
        return;
    _gui.setTrantorianActionBubble(id, resourceTextures[resourceId], 5.0f * 7.0f / _world.getTime());
    std::cout << "Trantorian #" << id << " collect " << resourceTextures[resourceId] << "." << std::endl;
}

void CommandHandler::handle_pdi(std::stringstream &ss)
{
    int id;
    char sharp;

    if (!(ss >> sharp >> id))
        return;
    _gui.triggerTrantorianDeath(id);
    std::cout << "Trantorian #" << id << " is dead" << std::endl;
}

void CommandHandler::handle_pic(std::stringstream &ss)
{
    int x;
    int y;
    int level;
    int id;
    char sharp;
    
    if (!(ss >> x >> y >> level))
        return;
    while (ss >> sharp >> id) {
        _gui.setTrantorianIncanting(id, true);
    }
    std::cout << "Incantation start at " << x << ":" << y << std::endl;
}

void CommandHandler::handle_pie(std::stringstream &ss)
{
    int x;
    int y;
    int result;

    if (!(ss >> x >> y >> result))
        return;
    _gui.stopIncantationAt(x, y);
    std::cout << "Incantation at " << x << ":" << y << " | result : " << result << std::endl;
}

void CommandHandler::handle_pbc(std::stringstream &ss)
{
    int id;
    char sharp;
    std::string message;

    if (!(ss >> sharp >> id >> message))
        return;
    _gui.addBroadcast(id);
    std::cout << "trantorian #" << id << ": \"" << message << "\"" << std::endl;
}

void CommandHandler::handle_pfk(std::stringstream &ss)
{
    int id;
    char sharp;

    if (!(ss >> sharp >> id))
        return;
    _gui.setTrantorianActionBubble(id, "egg", 5.0f * 42.0f / _world.getTime()); 
    std::cout << "Egg laying by the trantorian #" << id << std::endl;
}

void CommandHandler::handle_pdr(std::stringstream &ss)
{
    int id;
    int nb;
    char sharp;

    if (!(ss >> sharp >> id >> nb))
        return;
    _gui.setTrantorianActionBubble(id, "ground", 5.0f * 42.0f / _world.getTime()); // à changer pour une main en mode il drop 
    std::cout << "Trantorian #" << id << " drop " << nb << " ressources." << std::endl;
}

void CommandHandler::handle_edi(std::stringstream &ss)
{
    int eggNB;
    char sharp;

    if (!(ss >> sharp >> eggNB))
        return;
    _world.removeEgg(eggNB);
    std::cout << "Egg #" << eggNB << " is dead" << std::endl;

}

void CommandHandler::handle_sst(std::stringstream &ss)
{
    int timeUnit;

    if (!(ss >> timeUnit))
        return;
    _world.setTimeUnit(timeUnit);
    std::cout << "Time unit : " << timeUnit << std::endl;
}

void CommandHandler::handle_seg(std::stringstream &ss)
{
    std::string teamName;

    if (!(ss >> teamName))
        return;
    _world.setGameOver(teamName);
    std::cout << "Team : " << teamName << " win.\nEnd of game" << std::endl;
}

void CommandHandler::handle_smg(std::stringstream &ss)
{
    std::string message;

    if (!(ss >> message))
        return;
    std::cout << "Server : \"" << message << "\"" << std::endl;
}

void CommandHandler::handle_plv(std::stringstream &ss)
{
    int id;
    int lvl;
    char sharp;

    if (!(ss >> sharp >> id >> lvl))
        return;
    _world.setTrantorianLvl(id, lvl);
    std::cout << "Trantorian #" << id << " reach the level " << lvl << std::endl;
}

void CommandHandler::handle_suc(std::stringstream &)
{
    std::cout << "unknown command by the server" << std::endl;
}

void CommandHandler::handle_sbp(std::stringstream &)
{
    std::cout << "wrong command parameter" << std::endl;
}