/*
** EPITECH PROJECT, 2026
** Client
** File description:
** Definition of the map of the game
*/

#ifndef MAP_HPP_
#define MAP_HPP_

#include "Tile.hpp"
#include <vector>

class Map {
    private:
        unsigned int _width;
        unsigned int _height;
        std::vector<Tile> _tiles;
    public:
        Map(unsigned int width, unsigned int height);
        Tile &getTile(unsigned int x, unsigned int y);
        const Tile &getTile(unsigned int x, unsigned int y) const;
        unsigned int getWidth() const;
        unsigned int getHeight() const;
        void spawnResources();
};

#endif /* !MAP_HPP_ */
