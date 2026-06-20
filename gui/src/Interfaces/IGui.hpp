/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** IGui
*/

#ifndef IGUI_HPP_
#define IGUI_HPP_

#include <string>

class IGui {
    public:
        virtual ~IGui() = default;

        virtual bool isOpen() const = 0;
        virtual void handleEvent() = 0;
        virtual void displayWindow() = 0;

        virtual void setPlayerActionBubble(int id, const std::string &textureKey, float duration) = 0;
        virtual void triggerPlayerDeath(int id) = 0;
        virtual void triggerPlayerBroadcast(int id, const std::string &message) = 0;
        virtual void setPlayerIncanting(int id, bool state) = 0;
        virtual void stopIncantationAt(int x, int y) = 0;
};

#endif /* !IGUI_HPP_ */
