/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RaylibGui
*/

#pragma once

#include "Interfaces/IGui.hpp"
#include "Encapsulations/RayWindow.hpp"
#include "Encapsulations/RayCamera.hpp"
#include "World.hpp"

class RaylibGui : public IGui {
    public:
        RaylibGui(World &world);
        ~RaylibGui() override = default;

        bool isOpen() const override;
        void handleEvent() override;
        void displayWindow() override;
        
        void setPlayerActionBubble(int id, const std::string &textureKey, float duration) override;
        void triggerPlayerDeath(int id) override;
        void setPlayerIncanting(int id, bool state) override;
        void stopIncantationAt(int x, int y) override;

    private:
        World &_world;
        RayWindow _window;
        RayCamera _camera;
};
