/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RenderMap
*/

#pragma once
#include "World.hpp"
#include "TextureManager.hpp"
#include "HandleTrantorians.hpp"

class RenderMap {
    public:
        RenderMap(TextureManager &textureManager, sf::RenderWindow  &window, World &world);
        ~RenderMap();
        void update(float tileSize, float offsetX, float offsetY);
        void drawBackground();
        void drawMap();
        HandleTrantorians &getHandleTrantorians();
    private:
        void drawFood(int x, int y, const TileData_t &tile);
        void drawOres(int x, int y, const TileData_t &tile);
        void drawEggs(int x, int y, const TileData_t &tile);
        void drawTileElements(int x, int y, const TileData_t &tile);
        void drawSprite(std::string textureKey, float size, int x, int y, float offsetX, float offsetY);

        TextureManager &_textureManager;
        sf::RenderWindow  &_window;
        World &_world;
        HandleTrantorians _handleTrantorians;
        float _tileSize;
        float _offsetX;
        float _offsetY;
};