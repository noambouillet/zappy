/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** EventHandler
*/

#include "EventHandler.hpp"

#include "EventHandler.hpp"
#include <algorithm>

EventHandler::EventHandler(sf::RenderWindow &window, sf::View &camera): _window(window), _camera(camera){}

void EventHandler::update(float windowWidth, float windowHeight, UIRender &uiRender)
{
    _windowWidth = windowWidth;
    _windowHeight = windowHeight;

    while (_window.pollEvent(_event)) {
        if (_event.type == sf::Event::Closed)
            _window.close();
        uiRender.handleEvent(_event);
        handleZoom();
    }
    handleMove();
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
            if (_camera.getSize().x < _windowWidth) {
                _camera.zoom(1.1f);
            } else {
                _camera.setSize(_windowWidth, _windowHeight);
            }
        }
    }
}
