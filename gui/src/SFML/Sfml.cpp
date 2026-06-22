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
    _eventHandler.update(WIDTH, HEIGHT, _uiRender, getHandleTrantorians(), _world, _tileSize, _offsetX, _offsetY);
}

void Sfml::displayWindow()
{
    float deltaTime = _clock.restart().asSeconds();

    if (_tileSize == 0.0f && _world.getMapSize().first != 0)
        updateDimensions();
    _world.updateGameTime(deltaTime);
    _renderMap.getHandleTrantorians().updateAnimations(deltaTime);
    _window.clear(sf::Color::Black);
    _window.setView(_camera);
    _renderMap.drawBackground();
    _renderMap.drawMap();
    _window.setView(_window.getDefaultView());
    _uiRender.displayUI();
    _window.display();
}

void Sfml::updateDimensions()
{
    if (_world.getMapSize().first == 0 || _world.getMapSize().second == 0)
        return;
    _tileSize = HEIGHT * 0.80f / _world.getMapSize().second;
    _offsetY = (HEIGHT - HEIGHT * 0.80f) / 2.0f;
    _offsetX = (WIDTH - _tileSize * _world.getMapSize().first) / 2.0f;
    _limitWindowWidth = WIDTH;
    _camera.setSize(WIDTH, HEIGHT);
    _camera.setCenter(WIDTH / 2.0f, HEIGHT / 2.0f);
    _renderMap.update(_tileSize, _offsetX, _offsetY);
}

HandleTrantorians &Sfml::getHandleTrantorians()
{
    return _renderMap.getHandleTrantorians();
}

void Sfml::setTrantorianActionBubble(int id, const std::string &textureKey, float duration)
{
    _renderMap.getHandleTrantorians().setTrantorianActionBubble(id, textureKey, duration);
}

void Sfml::triggerTrantorianDeath(int id)
{
    _renderMap.getHandleTrantorians().triggerTrantorianDeath(id);
}

void Sfml::setTrantorianIncanting(int id, bool state)
{
    _renderMap.getHandleTrantorians().setTrantorianIncanting(id, state);
}

void Sfml::stopIncantationAt(int x, int y)
{
    _renderMap.getHandleTrantorians().stopIncantationAt(x, y);
}
