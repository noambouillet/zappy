/*
** EPITECH PROJECT, 2026
** Client
** File description:
** Class of the client connecting to the server
*/

#include "Client.hpp"

namespace ZappyServer {

Client::Client(int fd) : _fd(fd), _state(ClientState::WAITING_TEAM), _x(0), _y(0), _direction(1), _level(1)
{
    for (int i = 0; i < 7; i++)
        _inventory[i] = 0;
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

void Client::setX(unsigned int x)
{
    _x = x;
}

void Client::setY(unsigned int y)
{
    _y = y;
}

void Client::setDirection(unsigned int direction)
{
    _direction = direction;
}

void Client::setLevel(unsigned int level)
{
    _level = level;
}

unsigned int Client::getX() const
{
    return _x;
}

unsigned int Client::getY() const
{
    return _y;
}

unsigned int Client::getDirection() const
{
    return _direction;
}

unsigned int Client::getLevel() const
{
    return _level;
}

unsigned int Client::getInventory(unsigned int index) const
{
    if (index >= 7)
        return 0;
    return _inventory[index];
}

void Client::setInventory(unsigned int index, unsigned int value)
{
    if (index < 7)
        _inventory[index] = value;
}

std::queue<QueuedCommand> &Client::getCommandQueue()
{
    return _commandQueue;
}

void Client::queueCommand(const std::string &cmd, unsigned int ticks)
{
    if (_commandQueue.size() < 10) {
        _commandQueue.push({cmd, ticks});
    }
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
