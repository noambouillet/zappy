/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** EventHandler
*/

#pragma once
#include <SFML/Graphics.hpp>

class EventHandler {
    public:
        EventHandler(sf::RenderWindow &window, sf::View &camera);
        ~EventHandler() = default;
        void update(float windowWidth);
    private:
        void handleMove();
        void handleZoom();

        sf::Event _event;
        sf::RenderWindow &_window;
        sf::View &_camera;
        float _windowWidth;
};

