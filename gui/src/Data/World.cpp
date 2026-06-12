/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** World
*/

#include "World.hpp"
#include <stdexcept>

void World::setMapSize(size_t width, size_t height)
{
    _mapSize = {width, height};
    _map.resize(height);
    for (size_t y = 0; y < height; ++y) {
        _map[y].resize(width);
        for (size_t x = 0; x < width; ++x) {
            _map[y][x].ressources.resize(7, 0);
            _map[y][x].players.clear();
            _map[y][x].eggs.clear();
        }
    }
}

void World::setTile(int x, int y, const std::vector<int> &ressources)
{
    _map[y][x].ressources = ressources;
}

void World::addEgg(const std::vector<int>& egg)
{
    if (egg.size() < 4) return;
    int x = egg[2];
    int y = egg[3];
    
    _map[y][x].eggs.push_back(egg);
}

void World::addPlayer(Player_t player)
{
    int x = player.x;
    int y = player.y;
    
    _map[y][x].players[player.id] = player;
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
    throw std::runtime_error("Player ID not found in TileData");
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
        if (found) break;
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