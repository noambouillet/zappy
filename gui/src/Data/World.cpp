/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** World
*/

#include "World.hpp"
#include "GuiExceptions.hpp"
#include <stdexcept>

void World::setMapSize(size_t width, size_t height)
{
    _mapSize = {width, height};
    _map.resize(height);
    for (size_t y = 0; y < height; y++) {
        _map[y].resize(width);
        for (size_t x = 0; x < width; x++) {
            _map[y][x].ressources.resize(NB_RESSOURCE, 0);
            _map[y][x].players.clear();
            _map[y][x].eggs.clear();
        }
    }
}

void World::setTile(int x, int y, const std::vector<int> &ressources)
{
    _map[y][x].ressources = ressources;
}

void World::addEgg(int eggNB, int playerID, int x, int y)
{
    _map[y][x].eggs[eggNB] = playerID;
}

void World::addPlayer(Player_t player)
{ 
    _map[player.y][player.x].players[player.id] = player;
}

void World::removeEgg(int eggNB)
{
    for (size_t y = 0; y < _mapSize.second; ++y) {
        for (size_t x = 0; x < _mapSize.first; ++x) {
            auto it = _map[y][x].eggs.find(eggNB);
            if (it != _map[y][x].eggs.end()) {
                _map[y][x].eggs.erase(it);
                return;
            }
        }
    }
    throw GuiException("Egg ID not found in TileData");
}

void World::removeTrantorian(int id)
{
    for (size_t y = 0; y < _mapSize.second; ++y) {
        for (size_t x = 0; x < _mapSize.first; ++x) {
            auto it = _map[y][x].players.find(id);
            if (it != _map[y][x].players.end()) {
                _map[y][x].players.erase(it);
                return;
            }
        }
    }
    throw GuiException("Player ID not found in TileData");
}


Player_t &World::getTrantorian(int id)
{
    for (size_t y = 0; y < _mapSize.second; ++y) {
        for (size_t x = 0; x < _mapSize.first; ++x) {
            auto it = _map[y][x].players.find(id);
            if (it != _map[y][x].players.end()) {
                return it->second;
            }
        }
    }
    throw GuiException("Player ID not found in TileData");
}

void World::movePlayer(int id, int newX, int newY, int orientation)
{
    int oldX = -1;
    int oldY = -1;
    bool found = false;

    for (size_t y = 0; y < _mapSize.second; ++y) {
        for (size_t x = 0; x < _mapSize.first; ++x) {
            if (_map[y][x].players.find(id) != _map[y][x].players.end()) {
                oldX = x;
                oldY = y;
                found = true;
                break;
            }
        }
        if (found)
            break;
    }
    if (!found)
        return;
    Player_t player = _map[oldY][oldX].players[id];
    player.x = newX;
    player.y = newY;
    player.orientation = orientation;
    if (oldX != newX || oldY != newY) {
        _map[oldY][oldX].players.erase(id);
        _map[newY][newX].players[id] = player;
    } else {
        _map[oldY][oldX].players[id] = player;
    }
}

void World::setTimeUnit(int timeUnit)
{ 
    _timeUnit = timeUnit;
}

void World::addTeam(const std::string& teamName)
{
    _teams.push_back(teamName);
}

const std::vector<std::vector<TileData_t>>& World::getMap() const
{
    return _map;
}

std::pair<size_t, size_t> World::getMapSize() const
{
    return _mapSize;
}

TileData_t &World::getTileData(int x, int y)
{
    return _map[y][x];
}

int World::getTime() const
{
    return _timeUnit;
}