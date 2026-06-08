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

constexpr size_t WIDTH = 1920;
constexpr size_t HEIGHT = 1080;
class Sfml {
    public:
        Sfml();
        ~Sfml();
        sf::RenderWindow &getWindow();
        void handleEvent();
        void displayWindow();
        void drawMap();
        void setMapSize(size_t width, size_t height);
        void setTile(int x, int y, std::vector<int> ressources);
        void drawRessources();
        void setTimeUnit(int TimeUnit);
    private:
        sf::RenderWindow  _window;
        std::pair<size_t, size_t> _mapSize;
        std::vector<std::vector<std::vector<int>>> _map;
        int _TimeUnit;

        sf::Texture _foodTexture;
        sf::Sprite  _foodSprite;

        void updateDimensions();
        float _tileSize = 0.0f;
        float _offsetX = 0.0f;
        float _offsetY = 0.0f;
};
