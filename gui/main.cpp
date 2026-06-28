/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** main
*/

#include "Parsing_gui.hpp"
#include <unistd.h>
#include <iostream>
#include <memory>
#include "RaylibGui.hpp"
#include "Sfml.hpp"
#include "CommandHandler.hpp"
#include "NetworkHandler.hpp"
#include "Poll.hpp"
#include "GuiExceptions.hpp"
#include "Logger.hpp"

int main(int ac, char **av, char **env)
{
    Parsing_gui parse;
    std::string server_buffer;
    World world;
    Poll netPoll;

    try {
        parse.isTTY(env);
        networkData_t data = parse.parse_args(ac, av);
        NetworkHandler network(data.port, data.ip);

        int fd = network.connect_to_server();
        server_buffer = network.read_from_server(fd);
        if (server_buffer == "WELCOME\n") {
            network.send_command(fd, "GRAPHIC");
        } else {
            return 84;
        }
        server_buffer.clear(); 
        std::unique_ptr<IGui> gui;
        if (data.use3D) {
            gui = std::make_unique<RaylibGui>(world);
        } else {
            gui = std::make_unique<Sfml>(world);
        }
        CommandHandler handler(world, *gui);

        netPoll.addFd(fd, POLLIN);
        while (gui->isOpen()) {
            gui->handleEvent();
            std::string pendingCmd = gui->getPendingCommand();
            if (!pendingCmd.empty() && fd != -1) {
                network.send_command(fd, pendingCmd);
            }
            if (netPoll.wait(0) < 0)
                throw GuiException("poll failed");
            const auto &fds = netPoll.getFds();
            if (!fds.empty()) {
                const struct pollfd &serverFdStruct = fds[0];
                try {
                    if (serverFdStruct.revents & (POLLHUP | POLLERR | POLLNVAL)) {
                        throw MinorNetworkException("Server disconnected.");
                    }
                } catch (const MinorNetworkException &e) {
                    logger.warn("Server disconnected.");
                    netPoll.removeFd(fd);
                    fd = -1;
                }
                if (fd != -1 && (serverFdStruct.revents & POLLIN)) {
                    try {
                        server_buffer += network.read_from_server(fd);
                        std::string msg;
                        while (network.extract_message(server_buffer, msg)) {
                            handler.handle(msg);
                        }
                    } catch (const GuiException &e) {
                        logger.warn("Server disconnected during read.");
                        netPoll.removeFd(fd);
                        fd = -1;
                    }
                }
            }
            gui->displayWindow();
        }
    } catch (const MajorGuiException &e) {
        logger.error(std::string("Error: ") + e.what());
        return 84;
    } catch (const std::exception &e) {
        logger.error(std::string("Error: ") + e.what());
        return 84;
    }
    return 0;
}