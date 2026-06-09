/*
** EPITECH PROJECT, 2026
** Client
** File description:
** Class of the client connecting to the server
*/

#include "Client.hpp"

namespace ZappyServer {

Client::Client(int fd) : _fd(fd), _state(ClientState::WAITING_TEAM)
{
}

int Client::getFd() const
{
    return _fd;
}

int Client::getState() const
{
    return _state;
}

const std::string &Client::getTeamName() const
{
    return _teamName;
}

std::string &Client::getReadBuffer()
{
    return _readBuffer;
}

void Client::setState(int newState)
{
    _state = newState;
}

void Client::setTeamName(const std::string &newTeamName)
{
    _teamName = newTeamName;
}

void Client::invalidate()
{
    _fd = -1;
}

bool Client::isDead() const
{
    return _fd == -1;
}

}
