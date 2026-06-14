/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** Sfml
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <utility>
#include <string>
#include <vector>
#include "World.hpp"
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include "EventHandler.hpp"

typedef struct PlayerAnim_s {
    int id;
    sf::Vector2f visualPos = {0.0f, 0.0f};
    sf::Vector2f targetPos = {0.0f, 0.0f};
    bool isMoving = false;
    int lastX = -1;
    int lastY = -1;
} PlayerAnim_t;

constexpr size_t WIDTH = 1920;
constexpr size_t HEIGHT = 1080;
class Sfml {
    public:
        Sfml(World &world);
        ~Sfml();
        sf::RenderWindow &getWindow();
        void handleEvent();
        void displayWindow();
        void drawMap();
        void setMapSize(size_t width, size_t height);
        void setTimeUnit(int TimeUnit);
        void drawTileElements(int x, int y, const TileData_t &tile);
        void drawBackground();
        void updateAnimations(float deltaTime);
        sf::Vector2f convertToPixels(int x, int y) const;
        void drawRessources(int x, int y, const TileData_t &tile);
        void drawEggs(int x, int y, const TileData_t &tile);
        void drawTrantorians(const TileData_t &tile);

    private:
        sf::RenderWindow  _window;
        World &_world;
        sf::View _camera;
        EventHandler _eventHandler;

        std::unordered_map<std::string, sf::Texture> _textures;
        std::unordered_map<std::string, sf::Sprite> _sprites;
        sf::Clock _clock;

        void updateDimensions();
        int loadTexture();
        float _limitWindowWidth = 0.0f;
        float _tileSize = 0.0f;
        float _offsetX = 0.0f;
        float _offsetY = 0.0f;
        std::map<int, PlayerAnim_t> _playerAnims;
};
