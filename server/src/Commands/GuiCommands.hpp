/*
** EPITECH PROJECT, 2026
** GUI Commands
** File description:
** Definition of the commands used for the GUI
*/

#ifndef GUICOMMANDS_HPP_
#define GUICOMMANDS_HPP_

#include <string>
#include <functional>

namespace ZappyServer {

static constexpr unsigned int GUI_COMMANDS_NUMBER = 6;
static constexpr unsigned int RESOURCES_NUMBER = 7;
class Server;
class Client;

struct GuiCommandEntry {
    std::string_view name;
    std::function<void(Client &, Server &, const std::string &)> handler;
};

namespace GuiCommands {

    void dispatch(Client &client, Server &server, const std::string &line);

    void msz(Client &client, Server &server, const std::string &args);
    void bct(Client &client, Server &server, const std::string &args);
    void bct_broadcast(Server &server, unsigned int x, unsigned int y);
    void mct(Client &client, Server &server, const std::string &args);
    void tna(Client &client, Server &server, const std::string &args);
    void sgt(Client &client, Server &server, const std::string &args);
    void sst(Client &client, Server &server, const std::string &args);

    // les events qu'on envoie
    void pnw(Server &server, int playerId);
    void ppo(Server &server, int playerId);
    void plv(Server &server, int playerId);
    void pin(Server &server, int playerId);
    void pex(Server &server, int playerId);
    void pbc(Server &server, int playerId, const std::string &msg);
    void pic(Server &server, unsigned int x, unsigned int y, int level);
    void pie(Server &server, unsigned int x, unsigned int y, bool result);
    void pfk(Server &server, int playerId);
    void pdr(Server &server, int playerId, int resource);
    void pgt(Server &server, int playerId, int resource);
    void pdi(Server &server, int playerId);
    void enw(Server &server, int eggId, int playerId, unsigned int x, unsigned int y);
    void ebo(Server &server, int eggId);
    void edi(Server &server, int eggId);
    void seg(Server &server, const std::string &teamName);
    void smg(Client &client, Server &server, const std::string &msg);
    void suc(Client &client, Server &server);
    void sbp(Client &client, Server &server);
}

}

#endif /* !GUICOMMANDS_HPP_ */
