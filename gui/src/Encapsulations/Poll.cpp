/*
** EPITECH PROJECT, 2026
** Poll
** File description:
** Encapsulation of poll
*/

#include "Poll.hpp"
#include "GuiExceptions.hpp"
#include <string>
#include <cerrno>

std::vector<struct pollfd>::iterator Poll::findFd(int fd)
{
    auto index = _fds.begin();

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
        throw GuiException("Poll fd already registered: " + std::to_string(fd) + ".");
    }
    pollDescriptor.fd = fd;
    pollDescriptor.events = events;
    pollDescriptor.revents = 0;
    _fds.push_back(pollDescriptor);
}

void Poll::removeFd(int fd)
{
    const auto index = findFd(fd);

    if (index == _fds.end()) {
        throw GuiException("Poll fd not found: " + std::to_string(fd) + ".");
    }
    _fds.erase(index);
}

int Poll::wait(int timeout)
{
    const int result = poll(_fds.data(), static_cast<nfds_t>(_fds.size()), timeout);

    if (result < 0 && errno != EINTR) {
        throw GuiException("poll failed.");
    }
    return result;
}

const std::vector<struct pollfd> &Poll::getFds() const
{
    return _fds;
}