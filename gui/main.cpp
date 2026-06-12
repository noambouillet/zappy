/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** main
*/

#include "Parsing_gui.hpp"
#include <poll.h>
#include <unistd.h>
#include <iostream>
#include "Sfml.hpp"
#include "CommandHandler.hpp"
#include "NetworkHandler.hpp"

int main(int ac, char **av)
{
    Parsing_gui parse;
    std::string server_buffer;
    World world;
    CommandHandler handler(world);

    try {
        networkData_t data = parse.parse_args(ac, av);
        NetworkHandler network(data.port, data.ip);
        Sfml gui(world);
        int fd = network.connect_to_server();
        server_buffer = network.read_from_server(fd);
        if (server_buffer == "WELCOME\n")
            network.send_command(fd, "GRAPHIC\n");
        else
            return 84;
        server_buffer.clear(); 
        struct pollfd poll_fd[1];
        poll_fd[0] = {fd, POLLIN, 0};
        while (gui.getWindow().isOpen()) {
            gui.handleEvent();
            if (poll(poll_fd, 1, 0) < 0)
                throw std::runtime_error("poll failed");
                
            if (poll_fd[0].revents & (POLLHUP | POLLERR | POLLNVAL)) {
                std::cout << "Server disconnected." << std::endl;
                break;
            }
            if (poll_fd[0].revents & POLLIN) {
                server_buffer += network.read_from_server(fd);
                std::string msg;
                while (network.extract_message(server_buffer, msg)) {
                    handler.handle(msg);
                }
            }
            gui.displayWindow();
        }
        close(fd);
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}