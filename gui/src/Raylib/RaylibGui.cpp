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
#include <filesystem>
#include <cmath>

RaylibGui::RaylibGui(World &world) : _world(world), _window(1920, 1080, "Zappy 3D - Raylib")
{
    _backgroundTexture = std::make_unique<RayTexture>("gui/assets/3D/backgrounds/background.png");
    
    for (const auto & entry : std::filesystem::directory_iterator("gui/assets/3D/textures")) {
        if (entry.is_regular_file()) {
            std::string name = entry.path().stem().string();
            _textures[name] = std::make_unique<RayTexture>(entry.path().string());
            std::cout << "Loaded texture: " << name << " from " << entry.path() << std::endl;
        }
    }

    for (const auto & entry : std::filesystem::directory_iterator("gui/assets/3D/models")) {
        if (entry.is_regular_file()) {
            std::string name = entry.path().stem().string();
            _models[name] = std::make_unique<RayModel>(entry.path().string());
            
            BoundingBox box = GetModelBoundingBox(_models[name]->getModel());
            float maxSize = std::max({box.max.x - box.min.x, box.max.y - box.min.y, box.max.z - box.min.z});
            _modelScales[name] = (maxSize > 0.0f) ? (0.20f / maxSize) : 0.20f;

            std::cout << "Loaded model: " << name << " from " << entry.path() << std::endl;
        }
    }
}

bool RaylibGui::isOpen() const
{
    return _window.isOpen();
}

void RaylibGui::handleEvent()
{
    _camera.update(GetFrameTime());
    updateAnimations(GetFrameTime());
}

void RaylibGui::updateAnimations(float deltaTime)
{
    for (auto it = _playerAnims.begin(); it != _playerAnims.end();) {
        RayPlayerAnim_t &anim = it->second;

        if (anim.isDying) {
            anim.deathTimer += deltaTime;
            if (anim.deathTimer >= 1.0f) {
                anim.isDeadAndGone = true;
                try {
                    _world.removeTrantorian(anim.id);
                } catch (...) {}
                it = _playerAnims.erase(it);
                continue;
            }
        }

        if (anim.isIncanting) {
            anim.incantTimer += deltaTime;
        }

        if (anim.bubbleTimer > 0.0f) {
            anim.bubbleTimer -= deltaTime;
            if (anim.bubbleTimer <= 0.0f && !anim.bubbleQueue.empty()) {
                anim.bubbleQueue.erase(anim.bubbleQueue.begin());
                if (!anim.bubbleQueue.empty()) {
                    anim.bubbleTimer = anim.bubbleQueue.front().second;
                }
            }
        }
        it++;
    }
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

    DrawTexturePro(_backgroundTexture->getTexture(), Rectangle{0, 0, static_cast<float>(_backgroundTexture->getTexture().width),static_cast<float>(_backgroundTexture->getTexture().height)}, Rectangle{0, 0, 1920, 1080}, Vector2{0, 0}, 0.0f, WHITE);
    _camera.beginMode3D();

    auto [width, height] = _world.getMapSize();
    if (width > 0 && height > 0) {
        _camera.setTarget((width - 1) / 2.0f, (height - 1) / 2.0f);
        for (size_t x = 0; x < width; x++) {
            for (size_t z = 0; z < height; z++) {
                drawCubeTexture(_textures["ground"]->getTexture(), Vector3{static_cast<float>(x), -0.5f, static_cast<float>(z)}, 1.0f, 1.0f, 1.0f, WHITE);
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
    RayPlayerAnim_t &anim = _playerAnims[id];
    anim.id = id;
    anim.bubbleQueue.push_back({textureKey, duration});
    if (anim.bubbleTimer <= 0.0f)
        anim.bubbleTimer = duration;
}

void RaylibGui::triggerPlayerDeath(int id)
{
    RayPlayerAnim_t &anim = _playerAnims[id];
    anim.id = id;
    anim.isDying = true;
    anim.deathTimer = 0.0f;
}

void RaylibGui::setPlayerIncanting(int id, bool state)
{
    RayPlayerAnim_t &anim = _playerAnims[id];
    anim.id = id;
    anim.isIncanting = state;
    if (state)
        anim.incantTimer = 0.0f;
}

void RaylibGui::stopIncantationAt(int x, int y)
{
    auto& tile = _world.getTileData(x, y);
    for (auto& [id, player] : tile.players) {
        if (_playerAnims.find(id) != _playerAnims.end()) {
            _playerAnims[id].isIncanting = false;
        }
    }
}

void RaylibGui::drawBubble(const RayPlayerAnim_t &anim, Vector3 position)
{
    if (anim.bubbleQueue.empty())
        return;
    std::string texKey = anim.bubbleQueue.front().first;
    if (_textures.find(texKey) != _textures.end()) {
        DrawBillboard(_camera.getCamera(), _textures[texKey]->getTexture(), position, 0.5f, WHITE);
    }
}

void RaylibGui::drawTileContent(int x, int z)
{
    auto& tile = _world.getTileData(x, z);
    if (!tile.ressources.empty() && tile.ressources[0] > 0) {
        if (_models.find("donut") != _models.end()) {
            DrawModel(_models["donut"]->getModel(), Vector3{static_cast<float>(x) - 0.3f, 0.05f, static_cast<float>(z) - 0.3f}, 0.35f, WHITE);
        }
    }

    std::string ressourceNames[] = {"", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};
    Vector3 offsets[] = {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, -0.3f},
        {0.3f, 0.0f, -0.3f},
        {-0.3f, 0.0f, 0.3f},
        {0.0f, 0.0f, 0.3f},
        {0.3f, 0.0f, 0.3f},
        {0.0f, 0.0f, 0.0f}
    };

    for (size_t i = 1; i < 7; ++i) {
        if (tile.ressources.size() > i && tile.ressources[i] > 0) {
            if (_models.find(ressourceNames[i]) != _models.end()) {
                DrawModel(_models[ressourceNames[i]]->getModel(), Vector3{static_cast<float>(x) + offsets[i].x, 0.15f, static_cast<float>(z) + offsets[i].z}, _modelScales[ressourceNames[i]], WHITE);
            }
        }
    }

    for (auto& [id, player] : tile.players) {
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
        RayPlayerAnim_t &anim = _playerAnims[id];
        anim.id = id;

        rlPushMatrix();
        float yOffset = 0.0f;
        if (anim.isIncanting)
            yOffset = std::sin(anim.incantTimer * 5.0f) * 0.2f;
        rlTranslatef(static_cast<float>(x), yOffset, static_cast<float>(z));
        rlRotatef(angle, 0.0f, 1.0f, 0.0f);
        if (anim.isIncanting)
            rlRotatef(anim.incantTimer * 180.0f, 0.0f, 1.0f, 0.0f);
        if (anim.isDying)
            rlRotatef(std::min(anim.deathTimer * 90.0f, 90.0f), 1.0f, 0.0f, 0.0f);
        rlScalef(0.5f, 0.5f, 0.5f);
        DrawModel(_models["wizard"]->getModel(), Vector3{0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
        rlPopMatrix();
        drawBubble(anim, Vector3{static_cast<float>(x), yOffset + 1.2f, static_cast<float>(z)});
    }
}
