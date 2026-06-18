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
    _models["donut"] = std::make_unique<RayModel>("gui/assets/3D/models/donut.glb");
    _models["wizard"] = std::make_unique<RayModel>("gui/assets/3D/models/wizard.glb");
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

    rlSetTexture(texture.id);
    rlBegin(RL_QUADS);
        rlColor4ub(color.r, color.g, color.b, color.a);

        rlNormal3f(0.0f, 0.0f, 1.0f);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);

        rlNormal3f(0.0f, 0.0f, -1.0f);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);

        rlNormal3f(0.0f, 1.0f, 0.0f);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);

        rlNormal3f(0.0f, -1.0f, 0.0f);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);

        rlNormal3f(1.0f, 0.0f, 0.0f);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);

        rlNormal3f(-1.0f, 0.0f, 0.0f);
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
        _camera.setTarget((width - 1) / 2.0f, (height - 1) / 2.0f);
        for (size_t x = 0; x < width; x++) {
            for (size_t z = 0; z < height; z++) {
                drawCubeTexture(_textures["ground"]->getTexture(), (Vector3){ (float)x, -0.5f, (float)z }, 1.0f, 1.0f, 1.0f, WHITE);
                drawTileContent(x, z);
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
    // TODO: Implement 3D death animation
    try {
        _world.removeTrantorian(id);
    } catch (const std::exception &e) {
    }
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

void RaylibGui::drawTileContent(int x, int z)
{
    auto& tile = _world.getTileData(x, z);
    if (!tile.ressources.empty() && tile.ressources[0] > 0) {
        DrawModel(_models["donut"]->getModel(), (Vector3){ (float)x - 0.3f, 0.05f, (float)z - 0.3f }, 0.35f, WHITE);
    }

    for (auto& [id, player] : tile.players) {
        (void)id;
        float angle = 0.0f;
        switch (player.orientation) {
            case 1: angle = 180.0f;
                break;
            case 2: angle = 90.0f;
                break;
            case 3: angle = 0.0f;
                break;
            case 4: angle = 270.0f;
                break;
            default: angle = 0.0f;
                break;
        }
        DrawModelEx(_models["wizard"]->getModel(), (Vector3){ (float)x, 0.0f, (float)z }, (Vector3){ 0.0f, 1.0f, 0.0f }, angle, (Vector3){ 0.5f, 0.5f, 0.5f }, WHITE);
    }
}
