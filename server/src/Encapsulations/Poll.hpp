/*
** EPITECH PROJECT, 2026
** Poll
** File description:
** Small wrapper around poll
*/

#ifndef POLL_HPP_
#define POLL_HPP_

#include <poll.h>
#include <vector>

namespace ZappyServer {

class Poll {
    private:
        std::vector<struct pollfd> _fds;
        std::vector<struct pollfd>::iterator findFd(int fd);
    public:
        void addFd(int fd, short events);
        void removeFd(int fd);
        void setEvents(int fd, short events);
        int wait(int timeout = -1);
        void clear();
        const std::vector<struct pollfd> &getFds() const;
};

}

#endif /* !POLL_HPP_ */
