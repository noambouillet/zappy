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

int main(int ac, char **av)
{
    Parsing_gui parse;
    std::string server_buffer;
    World world;
    Poll netPoll;

    try {
        networkData_t data = parse.parse_args(ac, av);
        NetworkHandler network(data.port, data.ip);

        std::unique_ptr<IGui> gui;
        if (data.use3D) {
            gui = std::make_unique<RaylibGui>(world);
        } else {
            gui = std::make_unique<Sfml>(world);
        }

        CommandHandler handler(world, *gui);
        
        int fd = network.connect_to_server();
        server_buffer = network.read_from_server(fd);
        if (server_buffer == "WELCOME\n") {
            network.send_command(fd, "GRAPHIC");
        } else {
            return 84;
        }
        server_buffer.clear(); 
        netPoll.addFd(fd, POLLIN);
        while (gui->isOpen()) {
            gui->handleEvent();
            if (netPoll.wait(0) < 0)
                throw GuiException("poll failed");
            const auto &fds = netPoll.getFds();
            if (!fds.empty()) {
                const struct pollfd &serverFdStruct = fds[0];
                if (serverFdStruct.revents & (POLLHUP | POLLERR | POLLNVAL)) {
                    std::cout << "Server disconnected." << std::endl;
                    break;
                }
                if (serverFdStruct.revents & POLLIN) {
                    server_buffer += network.read_from_server(fd);
                    std::string msg;
                    while (network.extract_message(server_buffer, msg)) {
                        handler.handle(msg);
                    }
                }
            }
            gui->displayWindow();
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}