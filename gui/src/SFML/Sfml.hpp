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
        void setTile(int x, int y, std::vector<int> ressources);
        void drawRessources();
        void drawEggs();
        void setTimeUnit(int TimeUnit);
        void addTeam(std::string teamName);
        void addEggs(std::vector<int> egg);
        void drawTrantorians();
        void drawBackground();
        void updateAnimations(float deltaTime);
        sf::Vector2f convertToPixels(int x, int y) const;

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
};
