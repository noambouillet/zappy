/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** EventHandler
*/

#include "EventHandler.hpp"

#include "EventHandler.hpp"
#include <algorithm>

EventHandler::EventHandler(sf::RenderWindow &window, sf::View &camera): _window(window), _camera(camera), _pendingCommand("") {}

void EventHandler::update(float windowWidth, float windowHeight, UIRender &uiRender, HandleTrantorians &handleTrantorians, World &world, float tileSize, float offsetX, float offsetY)
{
    sf::Vector2i mousePosPos = sf::Mouse::getPosition(_window);
    sf::Vector2f worldMousePos = _window.mapPixelToCoords(mousePosPos, _camera);
    int tileX = static_cast<int>((worldMousePos.x - offsetX) / tileSize);
    int tileY = static_cast<int>((worldMousePos.y - offsetY) / tileSize);
    auto mapSize = world.getMapSize();

    _windowWidth = windowWidth;
    _windowHeight = windowHeight;
    if (tileX >= 0 && tileX < static_cast<int>(mapSize.first) && tileY >= 0 && tileY < static_cast<int>(mapSize.second))
        world.setHoveredTile(tileX, tileY);
    else
        world.setHoveredTile(-1, -1);
    while (_window.pollEvent(_event)) {
        if (_event.type == sf::Event::Closed || (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::Escape))
            _window.close();
        uiRender.handleEvent(_event);
        handleTrantorians.handleEvent(_event);
        handleZoom();
        if (_event.type == sf::Event::MouseButtonReleased && _event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f uiMousePos = _window.mapPixelToCoords(mousePosPos, _window.getDefaultView());
            if (uiMousePos.x < 1500.0f)
                world.setSelectedTile(world.getHoveredTile().first, world.getHoveredTile().second);
        }
        if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::Space) {
            _pendingCommand = "sps";
        }
    }
    handleMove();
}

std::string EventHandler::getPendingCommand()
{
    std::string cmd = _pendingCommand;
    _pendingCommand.clear();
    return cmd;
}

void EventHandler::handleMove()
{
    float speed = 15.0f;
    sf::Vector2f movement(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        movement.y -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        movement.y += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        movement.x -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        movement.x += speed;
    if (movement.x != 0.0f || movement.y != 0.0f) {
        _camera.move(movement);
        float halfCamX = _camera.getSize().x / 2.0f;
        float halfCamY = _camera.getSize().y / 2.0f;
        float clampedX = std::clamp(_camera.getCenter().x, halfCamX, _windowWidth - halfCamX);
        float clampedY = std::clamp(_camera.getCenter().y, halfCamY, _windowHeight - halfCamY);
        _camera.setCenter(clampedX, clampedY);
    }
}

void EventHandler::handleZoom()
{
    if (_event.type == sf::Event::MouseWheelScrolled && _event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        if (_event.mouseWheelScroll.delta > 0) {
            if (_camera.getSize().x > _windowWidth * 0.1)
                _camera.zoom(0.9f);
        } else if (_event.mouseWheelScroll.delta < 0) {
            if (_camera.getSize().x < _windowWidth)
                _camera.zoom(1.1f);
            else
                _camera.setSize(_windowWidth, _windowHeight);
        }
    }
}
