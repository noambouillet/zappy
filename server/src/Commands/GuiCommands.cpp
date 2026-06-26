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
#include <optional>
#include "Logger.hpp"

namespace ZappyServer {

static const std::array<GuiCommandEntry, GUI_COMMANDS_NUMBER> guiDispatch = {{
    { "msz", GuiCommands::msz },
    { "bct", GuiCommands::bct },
    { "mct", GuiCommands::mct },
    { "tna", GuiCommands::tna },
    { "sgt", GuiCommands::sgt },
    { "sst", GuiCommands::sst },
    { "sps", GuiCommands::sps },
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
    logger.warn("Client fd " + std::to_string(client.getFd()) + " sent unknown GUI command: " + cmd);
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
    for (unsigned int i = 0; i < RESOURCES_NUMBER; i++)
        msg += " " + std::to_string(tile.resources[i]);
    msg += "\n";
    server.getSocket().sendMessage(client.getFd(), msg.c_str(), msg.size());
}

void GuiCommands::bct_broadcast(Server &server, unsigned int x, unsigned int y)
{
    Tile &tile = server.getMap().getTile(x, y);
    std::string msg  = "bct " + std::to_string(x) + " " + std::to_string(y);
    for (unsigned int i = 0; i < RESOURCES_NUMBER; i++)
        msg += " " + std::to_string(tile.resources[i]);
    msg += "\n";
    for (Client &client : server.getClients()) {
        if (!client.isDead() && client.getState() == ClientState::GUI) {
            server.getSocket().sendMessage(client.getFd(), msg.c_str(), msg.size());
        }
    }
}

void GuiCommands::mct(Client &client, Server &server, const std::string &)
{
    for (unsigned int y = 0; y < server.getHeight(); y++) {
        for (unsigned int x = 0; x < server.getWidth(); x++) {
            GuiCommands::bct(client, server, std::to_string(x) + " " + std::to_string(y));
        }
    }
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

void GuiCommands::sps(Client &, Server &server, const std::string &)
{
    if (server.isPaused())
        server.resume();
    else
        server.pause();
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

namespace {
    void broadcastToGui(Server &server, const std::string &msg) {
        for (Client &client : server.getClients()) {
            if (!client.isDead() && client.getState() == ClientState::GUI) {
                server.getSocket().sendMessage(client.getFd(), msg.c_str(), msg.size());
            }
        }
    }

    std::string prefixedId(int id) {
        return "#" + std::to_string(id);
    }

    std::optional<std::reference_wrapper<Client>> getClientById(Server &server, int id) {
        for (Client &client : server.getClients()) {
            if (client.getFd() == id) {
                return client;
            }
        }
        return std::nullopt;
    }
}

void GuiCommands::pnw(Server &server, int playerId)
{
    auto clientOpt = getClientById(server, playerId);
    if (!clientOpt.has_value())
        return;
    Client &client = clientOpt.value().get();
    std::string msg = "pnw " + prefixedId(playerId) + " " + std::to_string(client.getPlayerData()->getX()) + " " + std::to_string(client.getPlayerData()->getY()) + " " + std::to_string(client.getPlayerData()->getDirection()) + " " + std::to_string(client.getPlayerData()->getLevel()) + " " + client.getTeamName() + "\n";
    broadcastToGui(server, msg);
}

void GuiCommands::ppo(Server &server, int playerId)
{
    auto clientOpt = getClientById(server, playerId);
    if (!clientOpt.has_value())
        return;
    Client &client = clientOpt.value().get();
    std::string msg = "ppo " + prefixedId(playerId) + " " + std::to_string(client.getPlayerData()->getX()) + " " + std::to_string(client.getPlayerData()->getY()) + " " + std::to_string(client.getPlayerData()->getDirection()) + "\n";
    broadcastToGui(server, msg);
}

void GuiCommands::plv(Server &server, int playerId)
{
    auto clientOpt = getClientById(server, playerId);
    if (!clientOpt.has_value())
        return;
    Client &client = clientOpt.value().get();
    std::string msg = "plv " + prefixedId(playerId) + " " + std::to_string(client.getPlayerData()->getLevel()) + "\n";
    broadcastToGui(server, msg);
}

void GuiCommands::pin(Server &server, int playerId)
{
    auto clientOpt = getClientById(server, playerId);
    if (!clientOpt.has_value())
        return;
    Client &client = clientOpt.value().get();
    std::string msg = "pin " + prefixedId(playerId) + " " + std::to_string(client.getPlayerData()->getX()) + " " + std::to_string(client.getPlayerData()->getY());
    for (unsigned int i = 0; i < RESOURCES_NUMBER; i++) {
        msg += " " + std::to_string(client.getPlayerData()->getInventory(i));
    }
    msg += "\n";
    broadcastToGui(server, msg);
}

void GuiCommands::pex(Server &server, int playerId)
{
    std::string msg = "pex " + prefixedId(playerId) + "\n";
    broadcastToGui(server, msg);
}

void GuiCommands::pbc(Server &server, int playerId, const std::string &msg)
{
    std::string out = "pbc " + prefixedId(playerId) + " " + msg + "\n";
    broadcastToGui(server, out);
}

void GuiCommands::pic(Server &server, unsigned int x, unsigned int y, int level)
{
    std::string msg = "pic " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(level);
    for (Client &c : server.getClients()) {
        if (!c.isDead() && c.getState() == ClientState::AI && c.getPlayerData()->getX() == x && c.getPlayerData()->getY() == y && c.getPlayerData()->getLevel() == (unsigned int)level) {
            msg += " " + prefixedId(c.getFd());
        }
    }
    msg += "\n";
    broadcastToGui(server, msg);
}

void GuiCommands::pie(Server &server, unsigned int x, unsigned int y, bool result)
{
    std::string msg = "pie " + std::to_string(x) + " " + std::to_string(y) + " " + (result ? "1" : "0") + "\n";
    broadcastToGui(server, msg);
}

void GuiCommands::pfk(Server &server, int playerId)
{
    std::string msg = "pfk " + prefixedId(playerId) + "\n";
    broadcastToGui(server, msg);
}

void GuiCommands::pdr(Server &server, int playerId, int resource)
{
    std::string msg = "pdr " + prefixedId(playerId) + " " + std::to_string(resource) + "\n";
    broadcastToGui(server, msg);
}

void GuiCommands::pgt(Server &server, int playerId, int resource)
{
    std::string msg = "pgt " + prefixedId(playerId) + " " + std::to_string(resource) + "\n";
    broadcastToGui(server, msg);
}

void GuiCommands::pdi(Server &server, int playerId)
{
    std::string msg = "pdi " + prefixedId(playerId) + "\n";
    broadcastToGui(server, msg);
}

void GuiCommands::enw(Server &server, int eggId, int playerId, unsigned int x, unsigned int y)
{
    std::string msg = "enw " + prefixedId(eggId) + " " + prefixedId(playerId) + " " + std::to_string(x) + " " + std::to_string(y) + "\n";
    broadcastToGui(server, msg);
}

void GuiCommands::ebo(Server &server, int eggId)
{
    std::string msg = "ebo " + prefixedId(eggId) + "\n";
    broadcastToGui(server, msg);
}

void GuiCommands::edi(Server &server, int eggId)
{
    std::string msg = "edi " + prefixedId(eggId) + "\n";
    broadcastToGui(server, msg);
}

void GuiCommands::seg(Server &server, const std::string &teamName)
{
    std::string msg = "seg " + teamName + "\n";
    broadcastToGui(server, msg);
}

void GuiCommands::sps_broadcast(Server &server, bool isPaused)
{
    std::string msg = "sps " + std::to_string(isPaused ? 1 : 0) + "\n";
    broadcastToGui(server, msg);
}

}
