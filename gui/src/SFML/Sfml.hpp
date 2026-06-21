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
#include "TextureManager.hpp"
#include "RenderMap.hpp"
#include "HandleTrantorians.hpp"
#include "UIRender.hpp"

constexpr size_t WIDTH = 1920;
constexpr size_t HEIGHT = 1080;
class Sfml {
    public:
        Sfml(World &world);
        ~Sfml();
        sf::RenderWindow &getWindow();
        void handleEvent();
        void displayWindow();
        void setTrantorianActionBubble(int id, const std::string &textureKey, float duration);
        void triggerTrantorianDeath(int id);
        void setTrantorianIncanting(int id, bool state);
        void stopIncantationAt(int x, int y);
        HandleTrantorians &getHandleTrantorians();
    private:
        sf::RenderWindow  _window;
        World &_world;
        sf::View _camera;
        EventHandler _eventHandler;
        TextureManager _textureManager;
        RenderMap _renderMap;
        UIRender _uiRender;
        sf::Clock _clock;

        void updateDimensions();
        void setMapSize(size_t width, size_t height);
        void setTimeUnit(int TimeUnit);
        void updateAnimations(float deltaTime);

        float _limitWindowWidth = 0.0f;
        float _tileSize = 0.0f;
        float _offsetX = 0.0f;
        float _offsetY = 0.0f;
};
