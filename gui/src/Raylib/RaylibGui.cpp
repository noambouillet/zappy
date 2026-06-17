/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RaylibGui
*/

#include "RaylibGui.hpp"
#include <iostream>
#include <algorithm>
#include <rlgl.h>

RaylibGui::RaylibGui(World &world) : _world(world), _window(1920, 1080, "Zappy 3D - Raylib")
{
}

bool RaylibGui::isOpen() const
{
    return _window.isOpen();
}

void RaylibGui::handleEvent()
{
    _camera.update(GetFrameTime());
}

void RaylibGui::displayWindow()
{
    _window.beginDrawing();
    _window.clearBackground(RAYWHITE);

    _camera.beginMode3D();
    
    auto [width, height] = _world.getMapSize();
    if (width > 0 && height > 0) {
        _camera.setTarget(width / 2.0f, height / 2.0f);
        rlPushMatrix();
            rlTranslatef(width / 2.0f, 0.0f, height / 2.0f);
            DrawGrid(std::max(width, height), 1.0f);
        rlPopMatrix();
    } else {
        DrawGrid(20, 1.0f);
    }
    _camera.endMode3D();
    DrawFPS(10, 10);
    _window.endDrawing();
}

void RaylibGui::setPlayerActionBubble(int id, const std::string &textureKey, float duration)
{
    (void)id;
    (void)textureKey;
    (void)duration;
    // TODO: Implement 3D bubbles
}

void RaylibGui::triggerPlayerDeath(int id)
{
    (void)id;
    // TODO: Implement 3D death animation
}

void RaylibGui::setPlayerIncanting(int id, bool state)
{
    (void)id;
    (void)state;
    // TODO: Implement 3D incantation
}

void RaylibGui::stopIncantationAt(int x, int y)
{
    (void)x;
    (void)y;
    // TODO: Implement stopping incantation
}
