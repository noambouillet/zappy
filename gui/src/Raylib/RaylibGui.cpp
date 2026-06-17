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
    _backgroundTexture = std::make_unique<RayTexture>("gui/assets/3D/backgrounds/background.png");
    _textures["ground"] = std::make_unique<RayTexture>("gui/assets/3D/textures/ground.png");
}

bool RaylibGui::isOpen() const
{
    return _window.isOpen();
}

void RaylibGui::handleEvent()
{
    _camera.update(GetFrameTime());
}

void RaylibGui::drawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    // Set desired texture to be enabled while drawing following vertex data
    rlSetTexture(texture.id);

    rlBegin(RL_QUADS);
        rlColor4ub(color.r, color.g, color.b, color.a);
        // Front Face
        rlNormal3f(0.0f, 0.0f, 1.0f);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);
        // Back Face
        rlNormal3f(0.0f, 0.0f, - 1.0f);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);
        // Top Face
        rlNormal3f(0.0f, 1.0f, 0.0f);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);
        // Bottom Face
        rlNormal3f(0.0f, - 1.0f, 0.0f);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);
        // Right face
        rlNormal3f(1.0f, 0.0f, 0.0f);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);
        // Left Face
        rlNormal3f( - 1.0f, 0.0f, 0.0f);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);
    rlEnd();

    rlSetTexture(0);
}

void RaylibGui::displayWindow()
{
    _window.beginDrawing();
    _window.clearBackground(RAYWHITE);

    DrawTexturePro(_backgroundTexture->getTexture(), (Rectangle){0, 0, (float)_backgroundTexture->getTexture().width, (float)_backgroundTexture->getTexture().height},(Rectangle){0, 0, 1920, 1080},(Vector2){0, 0}, 0.0f, WHITE);
    _camera.beginMode3D();
    
    auto [width, height] = _world.getMapSize();
    if (width > 0 && height > 0) {
        _camera.setTarget(width / 2.0f, height / 2.0f);
        for (size_t x = 0; x < width; x++) {
            for (size_t z = 0; z < height; z++) {
                drawCubeTexture(_textures["ground"]->getTexture(), (Vector3){ (float)x, -0.5f, (float)z }, 1.0f, 1.0f, 1.0f, WHITE);
            }
        }
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
