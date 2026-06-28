/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RaylibGui
*/

#include "RaylibGui.hpp"
#include "Logger.hpp"
#include <algorithm>
#include <rlgl.h>
#include <filesystem>
#include <cmath>

RaylibGui::RaylibGui(World &world) : _world(world), _window(1280, 720, "Zappy 3D - Raylib"), _pendingCommand("")
{
    _backgroundTexture = std::make_unique<RayTexture>("gui/assets/3D/backgrounds/background.png");

    for (const auto & entry : std::filesystem::directory_iterator("gui/assets/3D/textures")) {
        if (entry.is_regular_file()) {
            std::string name = entry.path().stem().string();
            try {
                _textures[name] = std::make_unique<RayTexture>(entry.path().string());
                logger.info("Loaded texture: " + name + " from " + entry.path().string());
            } catch (const std::exception &e) {
                logger.warn("Warning: " + std::string(e.what()));
            }
        }
    }

    for (const auto & entry : std::filesystem::directory_iterator("gui/assets/3D/models")) {
        if (entry.is_regular_file()) {
            std::string name = entry.path().stem().string();
            try {
                bool loadAnims = (name.find("wizzard") != std::string::npos);
                _models[name] = std::make_unique<RayModel>(entry.path().string(), loadAnims);
                BoundingBox box = GetModelBoundingBox(_models[name]->getModel());
                float maxSize = std::max({box.max.x - box.min.x, box.max.y - box.min.y, box.max.z - box.min.z});
                _modelScales[name] = (maxSize > 0.0f) ? (0.20f / maxSize) : 0.20f;
                logger.info("Loaded model: " + name + " from " + entry.path().string());
            } catch (const std::exception &e) {
                logger.warn("Warning: " + std::string(e.what()));
            }
        }
    }
    _ui = std::make_unique<RayUI>(_world, _textures);

    InitAudioDevice();
    if (IsAudioDeviceReady()) {
        _backgroundMusic = LoadMusicStream("gui/assets/sounds/music.wav");
        if (_backgroundMusic.stream.buffer != nullptr) {
            PlayMusicStream(_backgroundMusic);
            SetMusicVolume(_backgroundMusic, 0.5f);
            _isMusicLoaded = true;
        }
    }
    _window.show();
}

RaylibGui::~RaylibGui()
{
    if (_isMusicLoaded) {
        UnloadMusicStream(_backgroundMusic);
    }
    if (IsAudioDeviceReady()) {
        CloseAudioDevice();
    }
}

bool RaylibGui::isOpen() const
{
    return _window.isOpen();
}

void RaylibGui::handleEvent()
{
    if (_isMusicLoaded) {
        UpdateMusicStream(_backgroundMusic);
    }

    float dt = GetFrameTime();
    _camera.update(dt);
    updateAnimations(dt);
    _world.updateGameTime(dt);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Ray ray = GetScreenToWorldRay(GetMousePosition(), _camera.getCamera());
        
        bool hitTrantorian = false;
        float closestDistance = 9999999.0f;
        auto mapSize = _world.getMapSize();

        for (const auto &[id, anim] : _playerAnims) {
            BoundingBox box;
            box.min = { anim.renderX - 0.4f, 0.0f, anim.renderZ - 0.4f };
            box.max = { anim.renderX + 0.4f, 2.0f, anim.renderZ + 0.4f };
            RayCollision collision = GetRayCollisionBox(ray, box);
            if (collision.hit && collision.distance < closestDistance) {
                closestDistance = collision.distance;
                _selectedTrantorianId = id;
                hitTrantorian = true;
            }
        }

        if (hitTrantorian) {
            _selectedTileX = -1;
            _selectedTileZ = -1;
        } else {
            _selectedTrantorianId = -1;
            float t = (-0.25f - ray.position.y) / ray.direction.y;
            if (t >= 0.0f) {
                float intersectX = ray.position.x + t * ray.direction.x;
                float intersectZ = ray.position.z + t * ray.direction.z;
                int x = static_cast<int>(std::round(intersectX));
                int z = static_cast<int>(std::round(intersectZ));

                if (x >= 0 && x < static_cast<int>(mapSize.first) && z >= 0 && z < static_cast<int>(mapSize.second)) {
                    _selectedTileX = x;
                    _selectedTileZ = z;
                } else {
                    _selectedTileX = -1;
                    _selectedTileZ = -1;
                }
            }
        }
    }

    if (IsKeyPressed(KEY_SPACE)) {
        _pendingCommand = "sps";
    }
}

std::string RaylibGui::getPendingCommand()
{
    std::string cmd = _pendingCommand;
    _pendingCommand.clear();
    return cmd;
}

void RaylibGui::updateAnimations(float deltaTime)
{
    for (auto it = _playerAnims.begin(); it != _playerAnims.end();) {
        RayPlayerAnim_t &anim = it->second;
        anim.animFrameCounter++;

        if (anim.bubbleTimer > 0.0f) {
            anim.bubbleTimer -= deltaTime;
            if (anim.bubbleTimer <= 0.0f && !anim.bubbleQueue.empty()) {
                anim.bubbleQueue.erase(anim.bubbleQueue.begin());
                if (!anim.bubbleQueue.empty()) {
                    anim.bubbleTimer = anim.bubbleQueue.front().second;
                    anim.animFrameCounter = 0;
                }
            }
        }

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

        if (anim.isBroadcasting) {
            anim.broadcastTimer += deltaTime;
            if (anim.broadcastTimer >= 1.5f) {
                anim.isBroadcasting = false;
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

    if (_world.isGameOver()) {
        _ui->drawGameOverScreen(_world.getWinningTeam());
        _window.endDrawing();
        return;
    }

    DrawTexturePro(_backgroundTexture->getTexture(), Rectangle{0, 0, static_cast<float>(_backgroundTexture->getTexture().width),static_cast<float>(_backgroundTexture->getTexture().height)}, Rectangle{0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())}, Vector2{0, 0}, 0.0f, WHITE);
    _camera.beginMode3D();

    auto mapSize = _world.getMapSize();
    if (mapSize.first > 0 && mapSize.second > 0) {
        _camera.setTarget((mapSize.first - 1) / 2.0f, (mapSize.second - 1) / 2.0f);
        for (size_t x = 0; x < mapSize.first; x++) {
            for (size_t z = 0; z < mapSize.second; z++) {
                drawCubeTexture(_textures["ground"]->getTexture(), Vector3{static_cast<float>(x), -0.5f, static_cast<float>(z)}, 1.0f, 1.0f, 1.0f, WHITE);
                if (static_cast<int>(x) == _selectedTileX && static_cast<int>(z) == _selectedTileZ) {
                    DrawCubeWires(Vector3{static_cast<float>(x), -0.5f, static_cast<float>(z)}, 1.05f, 1.05f, 1.05f, GREEN);
                }
                drawTileContent(x, z);
            }
        }
    } else {
        DrawGrid(20, 1.0f);
    }
    _camera.endMode3D();

    if (mapSize.first > 0 && mapSize.second > 0) {
        for (size_t z = 0; z < mapSize.second; z++) {
            for (size_t x = 0; x < mapSize.first; x++) {
                auto &tile = _world.getTileData(x, z);
                for (const auto &[id, trantorian] : tile.trantorians) {
                    if (_playerAnims.find(id) != _playerAnims.end()) {
                        RayPlayerAnim_t &anim = _playerAnims[id];
                        float rX = anim.hasInitializedPos ? anim.renderX : static_cast<float>(x);
                        float rZ = anim.hasInitializedPos ? anim.renderZ : static_cast<float>(z);
                        Vector3 pos3D = {rX, 1.1f, rZ};
                        Vector2 pos2D = GetWorldToScreen(pos3D, _camera.getCamera());

                        Color lvlColor = RayUI::getLevelColor(trantorian.level);
                        std::string lvlText = "Lvl." + std::to_string(trantorian.level);
                        int textWidth = MeasureText(lvlText.c_str(), 15);
                        
                        DrawRectangle(pos2D.x - textWidth/2 - 20, pos2D.y - 7, 15, 15, lvlColor);
                        DrawRectangleLines(pos2D.x - textWidth/2 - 20, pos2D.y - 7, 15, 15, BLACK);
                        
                        DrawText(lvlText.c_str(), pos2D.x - textWidth/2, pos2D.y - 8, 15, BLACK);
                        DrawText(lvlText.c_str(), pos2D.x - textWidth/2 - 1, pos2D.y - 9, 15, WHITE);
                    }
                }
            }
        }
    }

    _ui->drawGlobalInfo();

    if (_selectedTrantorianId != -1)
        _ui->drawTrantorianInfo(_selectedTrantorianId, GetMousePosition());
    else if (_selectedTileX != -1 && _selectedTileZ != -1)
        _ui->drawTileInfo(_selectedTileX, _selectedTileZ, GetMousePosition());
    DrawFPS(GetScreenWidth() - 100, 10);
    _window.endDrawing();
}

void RaylibGui::setTrantorianActionBubble(int id, const std::string &textureKey, float duration)
{
    RayPlayerAnim_t &anim = _playerAnims[id];
    anim.id = id;
    anim.bubbleQueue.push_back({textureKey, duration});
    if (anim.bubbleTimer <= 0.0f) {
        anim.bubbleTimer = duration;
        anim.animFrameCounter = 0;
    }
}

void RaylibGui::triggerTrantorianDeath(int id)
{
    RayPlayerAnim_t &anim = _playerAnims[id];
    anim.id = id;
    anim.isDying = true;
    anim.deathTimer = 0.0f;
    anim.animFrameCounter = 0;
}

void RaylibGui::setTrantorianIncanting(int id, bool state)
{
    RayPlayerAnim_t &anim = _playerAnims[id];
    anim.id = id;
    anim.isIncanting = state;
    if (state) {
        anim.incantTimer = 0.0f;
        anim.animFrameCounter = 0;
    }
}

void RaylibGui::stopIncantationAt(int x, int y)
{
    auto& tile = _world.getTileData(x, y);
    for (auto& [id, player] : tile.trantorians) {
        if (_playerAnims.find(id) != _playerAnims.end()) {
            _playerAnims[id].isIncanting = false;
        }
    }
}

void RaylibGui::drawBubble(const RayPlayerAnim_t &anim, Vector3 position)
{
    if (anim.bubbleQueue.empty())
        return;
    std::string key = anim.bubbleQueue.front().first;

    if (_models.find(key) != _models.end()) {
        rlPushMatrix();
        float time = GetTime();
        float yOffset = std::sin(time * 3.0f) * 0.1f;
        rlTranslatef(position.x, position.y + yOffset, position.z);
        rlRotatef(time * 90.0f, 0.0f, 1.0f, 0.0f);

        float scale = _modelScales[key] * 1.5f;
        DrawModel(_models[key]->getModel(), Vector3{0.0f, 0.0f, 0.0f}, scale, WHITE);
        rlPopMatrix();
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

    for (size_t i = 1; i < 7; i++) {
        if (tile.ressources.size() > i && tile.ressources[i] > 0) {
            if (_models.find(ressourceNames[i]) != _models.end()) {
                DrawModel(_models[ressourceNames[i]]->getModel(), Vector3{static_cast<float>(x) + offsets[i].x, 0.15f, static_cast<float>(z) + offsets[i].z}, _modelScales[ressourceNames[i]], WHITE);
            }
        }
    }

    int eggCount = 0;
    for (const auto& [eggId, trantorianId] : tile.eggs) {
        float offsetX = 0.25f * std::cos(eggCount * 2.0f);
        float offsetZ = 0.25f * std::sin(eggCount * 2.0f);
        
        Color eggColor = WHITE;
        try {
            Trantorian_t t = _world.getTrantorian(trantorianId);
            eggColor = RayUI::getTeamColor(t.teamName);
        } catch (...) {}

        if (_models.find("egg") != _models.end()) {
            DrawModel(_models["egg"]->getModel(), Vector3{static_cast<float>(x) + offsetX, 0.10f, static_cast<float>(z) + offsetZ}, _modelScales["egg"] * 3.0f, eggColor);
            eggCount++;
        }
    }

    for (auto& [id, trantorian] : tile.trantorians) {
        float angle = 0.0f;
        switch (trantorian.orientation) {
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

        if (!anim.hasInitializedPos) {
            anim.renderX = static_cast<float>(x);
            anim.renderZ = static_cast<float>(z);
            anim.hasInitializedPos = true;
        }

        float dt = GetFrameTime();
        float speed = 7.0f;
        float targetX = static_cast<float>(x);
        float targetZ = static_cast<float>(z);
        float diffX = targetX - anim.renderX;
        float diffZ = targetZ - anim.renderZ;

        if (std::abs(diffX) > 1.5f || std::abs(diffZ) > 1.5f) {
            anim.renderX = targetX;
            anim.renderZ = targetZ;
            diffX = 0;
            diffZ = 0;
        }

        if (std::abs(diffX) > 0.01f) {
            if (std::abs(diffX) > speed * dt) {
                anim.renderX += (diffX > 0 ? 1 : -1) * speed * dt;
            } else {
                anim.renderX = targetX;
            }
            anim.isMoving = true;
        } else if (std::abs(diffZ) > 0.01f) {
            if (std::abs(diffZ) > speed * dt) {
                anim.renderZ += (diffZ > 0 ? 1 : -1) * speed * dt;
            } else {
                anim.renderZ = targetZ;
            }
            anim.isMoving = true;
        } else {
            anim.renderX = targetX;
            anim.renderZ = targetZ;
            anim.isMoving = false;
        }

        if (anim.isMoving) {
            anim.bubbleTimer = 0.0f;
            anim.bubbleQueue.clear();
        }

        rlPushMatrix();
        float yOffset = 0.0f;
        rlTranslatef(anim.renderX, yOffset, anim.renderZ);
        rlRotatef(angle, 0.0f, 1.0f, 0.0f);
        if (anim.isDying)
            rlRotatef(std::min(anim.deathTimer * 90.0f, 90.0f), 1.0f, 0.0f, 0.0f);
        rlScalef(0.5f, 0.5f, 0.5f);
        Color wizardColor = RayUI::getTeamColor(trantorian.teamName);
        std::string modelName = "wizzard_walking";

        if (anim.isDying)
            modelName = "wizzard_dead";
        else if (anim.isIncanting)
            modelName = "wizzard_evolving";
        else if (anim.isMoving)
            modelName = "wizzard_walking";
        else if (anim.bubbleTimer > 0.0f)
            modelName = "wizzard_collecting";
        else if (_models.find("wizzard_idle") != _models.end())
            modelName = "wizzard_idle";

        if (_models.find(modelName) != _models.end()) {
            if (_models[modelName]->hasAnimations()) {
                if (anim.isMoving || modelName != "wizzard_walking") {
                    bool loopAnim = (modelName != "wizzard_evolving");
                    int frame = anim.animFrameCounter;
                    if (modelName == "wizzard_evolving" && frame > 200) {
                        frame = 200;
                    }
                    _models[modelName]->updateAnimation(0, frame, loopAnim);
                } else {
                    _models[modelName]->updateAnimation(0, 0);
                }
            }
            DrawModel(_models[modelName]->getModel(), Vector3{0.0f, 0.0f, 0.0f}, 0.010f, wizardColor);
        }
        rlPopMatrix();

        if (anim.isBroadcasting) {
            float radius = anim.broadcastTimer * 5.0f;
            float alpha = 1.0f - (anim.broadcastTimer / 1.5f);
            if (alpha < 0.0f)
                alpha = 0.0f;
            Color waveColor = Color{0, 121, 241, static_cast<unsigned char>(255 * alpha)};
            DrawCircle3D(Vector3{anim.renderX, 0.5f, anim.renderZ}, radius, Vector3{1.0f, 0.0f, 0.0f}, 90.0f, waveColor);
        }

        drawBubble(anim, Vector3{anim.renderX, yOffset + 1.6f, anim.renderZ});
    }
}

void RaylibGui::addBroadcast(int id)
{
    RayPlayerAnim_t &anim = _playerAnims[id];
    anim.id = id;
    anim.isBroadcasting = true;
    anim.broadcastTimer = 0.0f;
}
