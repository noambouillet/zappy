/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** Sfml
*/

#include "Sfml.hpp"
#include <algorithm>
#include <cmath>
#include "GuiExceptions.hpp"

Sfml::Sfml(World &world): _window(sf::VideoMode::getDesktopMode(), "trantor", sf::Style::Titlebar | sf::Style::Close), _world(world), _eventHandler(_window, _camera), _renderMap(_textureManager, _window, _world)
{
    _window.setFramerateLimit(60);
    _textureManager.loadAllTextures();
}

Sfml::~Sfml()
{
}

sf::RenderWindow &Sfml::getWindow()
{
    return _window;
}

void Sfml::handleEvent()
{
    _eventHandler.update(_limitWindowWidth);
}

void Sfml::displayWindow()
{
    float deltaTime = _clock.restart().asSeconds();

    if (_tileSize == 0.0f && _world.getMapSize().first != 0)
        updateDimensions();
    _renderMap.getHandleTrantorians().updateAnimations(deltaTime);

    _window.clear(sf::Color::Black);
    _window.setView(_camera); //c'est pour afficher les éléments
    _renderMap.drawBackground();
    _renderMap.drawMap();

    //_window.setView(_window.getDefaultView()); // et ça c'est pour le UI
    _window.display();
}

void Sfml::updateDimensions()
{
    if (_world.getMapSize().first == 0 || _world.getMapSize().second == 0)
        return;
    float currentWindowWidth = static_cast<float>(_window.getSize().x);
    float currentWindowHeight = static_cast<float>(_window.getSize().y);
    float mapAreaHeight = currentWindowHeight * 0.80f;
    _tileSize = mapAreaHeight / _world.getMapSize().second;
    float finalMapWidth = _tileSize * _world.getMapSize().first;
    _offsetY = (currentWindowHeight - mapAreaHeight) / 2.0f;
    _offsetX = (currentWindowWidth - finalMapWidth) / 2.0f;
    _limitWindowWidth = currentWindowWidth;
    _camera.setSize(currentWindowWidth, currentWindowHeight);
    _camera.setCenter(currentWindowWidth / 2.0f, currentWindowHeight / 2.0f);
    _renderMap.update(_tileSize, _offsetX, _offsetY);
}

HandleTrantorians &Sfml::getHandleTrantorians()
{
    return _renderMap.getHandleTrantorians();
}

