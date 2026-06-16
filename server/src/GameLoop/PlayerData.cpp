/*
** EPITECH PROJECT, 2026
** PlayerData
** File description:
** PlayerData class implementation
*/

#include "PlayerData.hpp"
#include "Logger.hpp"

namespace ZappyServer {

PlayerData::PlayerData() : _x(0), _y(0), _direction(1), _level(1), _foodTicks(126)
{
    for (int index = 0; index < 7; index++) {
        _inventory[index] = 0;
    }
}

void PlayerData::setX(unsigned int x)
{
    _x = x;
}

void PlayerData::setY(unsigned int y)
{
    _y = y;
}

void PlayerData::setDirection(unsigned int direction)
{
    _direction = direction;
}

void PlayerData::setLevel(unsigned int level)
{
    _level = level;
}

unsigned int PlayerData::getX() const
{
    return _x;
}

unsigned int PlayerData::getY() const
{
    return _y;
}

unsigned int PlayerData::getDirection() const
{
    return _direction;
}

unsigned int PlayerData::getLevel() const
{
    return _level;
}

unsigned int PlayerData::getInventory(unsigned int index) const
{
    if (index >= 7) {
        return 0;
    }
    return _inventory[index];
}

void PlayerData::setInventory(unsigned int index, unsigned int value)
{
    if (index < 7) {
        _inventory[index] = value;
    }
}

unsigned int PlayerData::getFoodTicks() const
{
    return _foodTicks;
}

void PlayerData::setFoodTicks(unsigned int value)
{
    _foodTicks = value;
}

std::queue<QueuedCommand> &PlayerData::getCommandQueue()
{
    return _commandQueue;
}

const std::queue<QueuedCommand> &PlayerData::getCommandQueue() const
{
    return _commandQueue;
}

void PlayerData::queueCommand(const std::string &cmd, unsigned int ticks)
{
    if (_commandQueue.size() < 10) {
        _commandQueue.push({cmd, ticks});
    } else {
        logger.warn("Command queue full, dropping command: " + cmd);
    }
}

}
