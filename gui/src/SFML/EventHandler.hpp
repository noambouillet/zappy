/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** EventHandler
*/

#ifndef EVENTHANDLER_HPP_
#define EVENTHANDLER_HPP_
#include <SFML/Graphics.hpp>
#include "UIRender.hpp"
#include "HandleTrantorians.hpp"

class EventHandler {
    public:
        EventHandler(sf::RenderWindow &window, sf::View &camera);
        ~EventHandler() = default;
        void update(float windowWidth, float windowHeight, UIRender &uiRender, HandleTrantorians &handleTrantorians, World &world, float tileSize, float offsetX, float offsetY);
        std::string getPendingCommand();
    private:
        void handleMove();
        void handleZoom();

        sf::Event _event;
        sf::RenderWindow &_window;
        sf::View &_camera;
        float _windowWidth;
        float _windowHeight;
        std::string _pendingCommand;
};


#endif /* !EVENTHANDLER_HPP_ */
