/*
** EPITECH PROJECT, 2026
** Poll
** File description:
** Encapsulation of poll
*/

#include "Poll.hpp"
#include "ServerExceptions.hpp"
#include <string>
#include <cerrno>

namespace ZappyServer {

std::vector<struct pollfd>::iterator Poll::findFd(int fd)
{
    std::vector<struct pollfd>::iterator index = _fds.begin();

    while (index != _fds.end()) {
        if (index->fd == fd) {
            return index;
        }
        index++;
    }
    return _fds.end();
}

void Poll::addFd(int fd, short events)
{
    struct pollfd pollDescriptor;

    if (findFd(fd) != _fds.end()) {
        throw PollException("Poll fd already registered: " + std::to_string(fd) + ".");
    }
    pollDescriptor.fd = fd;
    pollDescriptor.events = events;
    pollDescriptor.revents = 0;
    _fds.push_back(pollDescriptor);
}

void Poll::removeFd(int fd)
{
    const std::vector<struct pollfd>::iterator index = findFd(fd);

    if (index == _fds.end()) {
        throw PollException("Poll fd not found: " + std::to_string(fd) + ".");
    }
    _fds.erase(index);
}

void Poll::setEvents(int fd, short events)
{
    const std::vector<struct pollfd>::iterator index = findFd(fd);

    if (index == _fds.end()) {
        throw PollException("Poll fd not found: " + std::to_string(fd) + ".");
    }
    index->events = events;
}

int Poll::wait(int timeout)
{
    const int result = poll(_fds.data(), static_cast<nfds_t>(_fds.size()), timeout);

    if (result < 0 && errno != EINTR) {
        throw FatalPollException("poll failed.");
    }
    return result;
}

void Poll::clear()
{
    _fds.clear();
}

const std::vector<struct pollfd> &Poll::getFds() const
{
    return _fds;
}

}
