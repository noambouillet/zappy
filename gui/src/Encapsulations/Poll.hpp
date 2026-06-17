/*
** EPITECH PROJECT, 2026
** Poll
** File description:
** Encapsulation of poll
*/

#pragma once

#include <vector>
#include <poll.h>

class Poll {
    public:
        Poll() = default;
        ~Poll() = default;

        void addFd(int fd, short events);
        void removeFd(int fd);
        int wait(int timeout);
        const std::vector<struct pollfd> &getFds() const;

    private:
        std::vector<struct pollfd> _fds;
        std::vector<struct pollfd>::iterator findFd(int fd);
};