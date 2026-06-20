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

Sfml::Sfml(World &world): _window(sf::VideoMode({WIDTH, HEIGHT}), "trantor", sf::Style::Default), _world(world), _eventHandler(_window, _camera), _renderMap(_textureManager, _window, _world), _uiRender(_world, _window, _textureManager)
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

bool Sfml::isOpen() const
{
    return _window.isOpen();
}

void Sfml::handleEvent()
{
    _eventHandler.update(WIDTH, HEIGHT, _uiRender);
}

void Sfml::displayWindow()
{
    float deltaTime = _clock.restart().asSeconds();

    if (_tileSize == 0.0f && _world.getMapSize().first != 0)
        updateDimensions();
    _world.updateGameTime(deltaTime);
    _renderMap.getHandleTrantorians().updateAnimations(deltaTime);

    _window.clear(sf::Color::Black);
    _window.setView(_camera); //c'est pour afficher les éléments
    _renderMap.drawBackground();
    _renderMap.drawMap();

    _window.setView(_window.getDefaultView()); // et ça c'est pour le UI
    _uiRender.displayUI();
    _window.display();
}

void Sfml::updateDimensions()
{
    if (_world.getMapSize().first == 0 || _world.getMapSize().second == 0)
        return;
    float mapAreaHeight = HEIGHT * 0.80f;
    _tileSize = mapAreaHeight / _world.getMapSize().second;
    float finalMapWidth = _tileSize * _world.getMapSize().first;
    _offsetY = (HEIGHT - mapAreaHeight) / 2.0f;
    _offsetX = (WIDTH - finalMapWidth) / 2.0f;
    _limitWindowWidth = WIDTH;
    _camera.setSize(WIDTH, HEIGHT);
    _camera.setCenter(WIDTH / 2.0f, HEIGHT / 2.0f);
    _renderMap.update(_tileSize, _offsetX, _offsetY);
}

HandleTrantorians &Sfml::getHandleTrantorians()
{
    return _renderMap.getHandleTrantorians();
}

void Sfml::setPlayerActionBubble(int id, const std::string &textureKey, float duration)
{
    _renderMap.getHandleTrantorians().setPlayerActionBubble(id, textureKey, duration);
}

void Sfml::triggerPlayerDeath(int id)
{
    _renderMap.getHandleTrantorians().triggerPlayerDeath(id);
}

void Sfml::triggerPlayerBroadcast(int id, const std::string &message)
{
    _renderMap.getHandleTrantorians().triggerPlayerBroadcast(id, message);
}

void Sfml::setPlayerIncanting(int id, bool state)
{
    _renderMap.getHandleTrantorians().setPlayerIncanting(id, state);
}

void Sfml::stopIncantationAt(int x, int y)
{
    _renderMap.getHandleTrantorians().stopIncantationAt(x, y);
}
