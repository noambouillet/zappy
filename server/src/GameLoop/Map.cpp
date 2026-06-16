/*
** EPITECH PROJECT, 2026
** Map
** File description:
** Code of the map
*/

#include "Map.hpp"
#include <cstdlib>
#include <algorithm>

namespace ZappyServer {

static const double DENSITIES[7] = {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};

Map::Map(unsigned int width, unsigned int height) : _width(width), _height(height), _tiles(width * height)
{
    spawnResources();
}

Tile &Map::getTile(unsigned int x, unsigned int y)
{
    return _tiles[(y % _height) * _width + (x % _width)];
}

const Tile &Map::getTile(unsigned int x, unsigned int y) const
{
    return _tiles[(y % _height) * _width + (x % _width)];
}

unsigned int Map::getWidth() const
{
    return _width;
}

unsigned int Map::getHeight() const
{
    return _height;
}

void Map::spawnResources()
{
    unsigned int total = _width * _height;

    for (int ressourceIndex = 0; ressourceIndex < 7; ressourceIndex++) {
        unsigned int count = static_cast<unsigned int>(total * DENSITIES[ressourceIndex]);
        if (count == 0)
            count = 1;
        for (unsigned int i = 0; i < count; i++) {
            unsigned int x = rand() % _width;
            unsigned int y = rand() % _height;
            Tile &tile = getTile(x, y);
            tile.resources[ressourceIndex]++;
        }
    }
}

}
