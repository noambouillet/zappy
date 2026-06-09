/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** World
*/

#include "World.hpp"

void World::setTile(int x, int y, const std::vector<int> &ressources)
{
    _map[x][y] = ressources;
}

void World::setTimeUnit(int TimeUnit)
{
    _timeUnit = TimeUnit;
}

void World::setMapSize(size_t width, size_t height)
{
    _mapSize = {width, height};
    _map.resize(height);
    for (size_t y = 0; y < height; ++y) {
        _map[y].resize(width);
        for (size_t x = 0; x < width; ++x) {
            _map[y][x].resize(7, 0);
        }
    }
}

void World::addTeam(const std::string &teamName)
{
    _teams.push_back(teamName);
}

void World::addEgg(const std::vector<int> &egg)
{
    _eggs.push_back(egg);
}

void World::addPlayer(Player_t player)
{
    _players[player.id] = player;
}
