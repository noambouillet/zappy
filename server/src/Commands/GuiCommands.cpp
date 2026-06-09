/*
** EPITECH PROJECT, 2026
** GUI Commands
** File description:
** Implementation of the commands used for the GUI
*/

#include "GuiCommands.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include <array>
#include <string_view>

namespace ZappyServer {

static const std::array<GuiCommandEntry, GUI_COMMANDS_NUMBER> guiDispatch = {{
    { "msz", GuiCommands::msz },
    { "bct", GuiCommands::bct },
    { "mct", GuiCommands::mct },
    { "tna", GuiCommands::tna },
    { "sgt", GuiCommands::sgt },
    { "sst", GuiCommands::sst },
}};

void GuiCommands::dispatch(Client &client, Server &server, const std::string &line)
{
    std::size_t spacePos = line.find(' ');
    std::string cmd = line.substr(0, spacePos);
    std::string args = spacePos != std::string::npos ? line.substr(spacePos + 1) : "";

    for (const auto &entry : guiDispatch) {
        if (entry.name == cmd) {
            entry.handler(client, server, args);
            return;
        }
    }
    GuiCommands::suc(client, server);
}

void GuiCommands::msz(Client &client, Server &server, const std::string &)
{
    std::string msg = "msz " + std::to_string(server.getWidth()) + " " + std::to_string(server.getHeight()) + "\n";
    server.getSocket().sendMessage(client.getFd(), msg.c_str(), msg.size());
}

void GuiCommands::bct(Client &client, Server &server, const std::string &args)
{
    unsigned int x = 0;
    unsigned int y = 0;

    std::sscanf(args.c_str(), "%u %u", &x, &y);
    Tile &tile = server.getMap().getTile(x, y);
    std::string msg  = "bct " + std::to_string(x) + " " + std::to_string(y);
    for (int i = 0; i < 7; i++)
        msg += " " + std::to_string(tile.resources[i]);
    msg += "\n";
    server.getSocket().sendMessage(client.getFd(), msg.c_str(), msg.size());
}

void GuiCommands::mct(Client &client, Server &server, const std::string &)
{
    for (unsigned int y = 0; y < server.getHeight(); y++)
        for (unsigned int x = 0; x < server.getWidth(); x++)
            GuiCommands::bct(client, server, "");
}

void GuiCommands::tna(Client &client, Server &server, const std::string &)
{
    for (const std::string &name : server.getTeamNames()) {
        std::string msg = "tna " + name + "\n";
        server.getSocket().sendMessage(client.getFd(), msg.c_str(), msg.size());
    }
}

void GuiCommands::sgt(Client &client, Server &server, const std::string &)
{
    std::string msg = "sgt " + std::to_string(server.getFreq()) + "\n";
    server.getSocket().sendMessage(client.getFd(), msg.c_str(), msg.size());
}

void GuiCommands::sst(Client &, Server &server, const std::string &args)
{
    unsigned int time = 0;
    std::sscanf(args.c_str(), "%u", &time);
    server.setFreq(time);
    std::string msg = "sst " + std::to_string(time) + "\n";
    for (Client &client : server.getClients()) {
        if (!client.isDead() && client.getState() == ClientState::GUI)
            server.getSocket().sendMessage(client.getFd(), msg.c_str(), msg.size());
    }
}

void GuiCommands::suc(Client &client, Server &server)
{
    server.getSocket().sendMessage(client.getFd(), "suc\n", 4);
}

void GuiCommands::sbp(Client &client, Server &server)
{
    server.getSocket().sendMessage(client.getFd(), "sbp\n", 4);
}

void GuiCommands::smg(Client &client, Server &server, const std::string &msg)
{
    std::string out = "smg " + msg + "\n";
    server.getSocket().sendMessage(client.getFd(), out.c_str(), out.size());
}

void GuiCommands::pnw(Server &server, int playerId)
{
    (void)server;
    (void)playerId;
}

void GuiCommands::ppo(Server &server, int playerId)
{
    (void)server;
    (void)playerId;
}

void GuiCommands::plv(Server &server, int playerId)
{
    (void)server;
    (void)playerId;
}

void GuiCommands::pin(Server &server, int playerId)
{
    (void)server;
    (void)playerId;
}

void GuiCommands::pex(Server &server, int playerId)
{
    (void)server;
    (void)playerId;
}

void GuiCommands::pbc(Server &server, int playerId, const std::string &msg)
{
    (void)server;
    (void)playerId;
    (void)msg;
}

void GuiCommands::pic(Server &server, unsigned int x, unsigned int y, int level)
{
    (void)server;
    (void)x;
    (void)y;
    (void)level;
}

void GuiCommands::pie(Server &server, unsigned int x, unsigned int y, bool result)
{
    (void)server;
    (void)x;
    (void)y;
    (void)result;
}

void GuiCommands::pfk(Server &server, int playerId)
{
    (void)server;
    (void)playerId;
}

void GuiCommands::pdr(Server &server, int playerId, int resource)
{
    (void)server;
    (void)playerId;
    (void)resource;
}

void GuiCommands::pgt(Server &server, int playerId, int resource)
{
    (void)server;
    (void)playerId;
    (void)resource;
}

void GuiCommands::pdi(Server &server, int playerId)
{
    (void)server;
    (void)playerId;
}

void GuiCommands::enw(Server &server, int eggId, int playerId, unsigned int x, unsigned int y)
{
    (void)server;
    (void)eggId;
    (void)playerId;
    (void)x;
    (void)y;
}

void GuiCommands::ebo(Server &server, int eggId)
{
    (void)server;
    (void)eggId;
}

void GuiCommands::edi(Server &server, int eggId)
{
    (void)server;
    (void)eggId;
}

void GuiCommands::seg(Server &server, const std::string &teamName)
{
    (void)server;
    (void)teamName;
}

}
