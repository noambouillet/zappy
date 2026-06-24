/*
** EPITECH PROJECT, 2026
** Map
** File description:
** Code of the map
*/

#include "Map.hpp"

namespace ZappyServer {

static const double DENSITIES[7] = {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};

static unsigned int getTargetCount(unsigned int totalTiles, int resourceIndex)
{
    unsigned int targetCount = static_cast<unsigned int>(totalTiles * DENSITIES[resourceIndex]);
    if (targetCount == 0)
        return 1;
    return targetCount;
}

Map::Map(unsigned int width, unsigned int height) : _width(width), _height(height), _tiles(width * height) {}

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

void Map::setSeed(unsigned int seed)
{
    _rng.seed(seed);
}

void Map::generate()
{
    spawnResources();
}

void Map::spawnResources()
{
    unsigned int total = _width * _height;
    std::uniform_int_distribution<unsigned int> distX(0, _width - 1);
    std::uniform_int_distribution<unsigned int> distY(0, _height - 1);

    for (int ressourceIndex = 0; ressourceIndex < 7; ressourceIndex++) {
        unsigned int count = getTargetCount(total, ressourceIndex);
        for (unsigned int i = 0; i < count; i++) {
            unsigned int x = distX(_rng);
            unsigned int y = distY(_rng);
            Tile &tile = getTile(x, y);
            tile.resources[ressourceIndex]++;
        }
    }
}

void Map::refillResources()
{
    unsigned int totalTiles = _width * _height;
    unsigned int currentCounts[7] = {0};
    std::uniform_int_distribution<unsigned int> distX(0, _width - 1);
    std::uniform_int_distribution<unsigned int> distY(0, _height - 1);

    for (const Tile &tile : _tiles) {
        for (int resourceIndex = 0; resourceIndex < 7; resourceIndex++)
            currentCounts[resourceIndex] += tile.resources[resourceIndex];
    }
    for (int resourceIndex = 0; resourceIndex < 7; resourceIndex++) {
        unsigned int targetCount = getTargetCount(totalTiles, resourceIndex);

        if (currentCounts[resourceIndex] >= targetCount)
            continue;
        for (unsigned int spawnIndex = currentCounts[resourceIndex]; spawnIndex < targetCount; spawnIndex++) {
            unsigned int x = distX(_rng);
            unsigned int y = distY(_rng);
            Tile &tile = getTile(x, y);
            tile.resources[resourceIndex]++;
        }
    }
}

}
