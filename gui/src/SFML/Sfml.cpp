/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** Sfml
*/

#include "Sfml.hpp"
#include <algorithm>
#include <cmath>
#include "GuiExceptions.hpp"

Sfml::Sfml(World &world): _window(sf::VideoMode::getDesktopMode(), "trantor", sf::Style::Titlebar | sf::Style::Close), _world(world), _eventHandler(_window, _camera)
{
    _window.setFramerateLimit(60);
    loadTexture();
}

Sfml::~Sfml()
{
}

sf::RenderWindow &Sfml::getWindow()
{
    return _window;
}


void Sfml::handleEvent()
{
    _eventHandler.update(_limitWindowWidth);
}

void Sfml::drawMap() 
{
    sf::RectangleShape tileShape(sf::Vector2f(_tileSize, _tileSize));
    tileShape.setOutlineColor(sf::Color::Black);
    tileShape.setOutlineThickness(-1.0f);

    auto tex = _textures.find("ground");
    if (tex != _textures.end()) {
        tileShape.setTexture(&tex->second);
        tileShape.setFillColor(sf::Color::White);
    } else {
        tileShape.setFillColor(sf::Color(34, 139, 34));
    }
    for (size_t y = 0; y < _world.getMapSize().second; ++y) {
        for (size_t x = 0; x < _world.getMapSize().first; ++x) {
            tileShape.setPosition(_offsetX + (x * _tileSize), _offsetY + (y * _tileSize));
            _window.draw(tileShape);
        }
    }
    for (size_t y = 0; y < _world.getMapSize().second; ++y) {
        for (size_t x = 0; x < _world.getMapSize().first; ++x) {
            drawTileElements(x, y, _world.getTileData(x, y));
        }
    }
}

void Sfml::drawSprite(std::string textureKey, float size, int x, int y, float offsetX, float offsetY)
{
    auto tex = _textures.find(textureKey);
    if (tex == _textures.end())
        return;
    float centerX = _offsetX + (x * _tileSize) + (_tileSize / offsetX);
    float centerY = _offsetY + (y * _tileSize) + (_tileSize / offsetY);
    const sf::Texture &texture = tex->second;
    _sprites[textureKey].setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    _sprites[textureKey].setScale(size / texture.getSize().x, size / texture.getSize().y);
    _sprites[textureKey].setPosition(centerX, centerY);
    _window.draw(_sprites[textureKey]);
}


void Sfml::drawFood(int x, int y, const TileData_t &tile)
{
    if (_tileSize <= 0.0f || tile.ressources.empty() || tile.ressources[0] <= 0)
        return;
    std::string textureKey = "donut";
    if (tile.ressources[0] >= 3 && tile.ressources[0] < 7)
        textureKey = "fewDonuts";
    else if (tile.ressources[0] >= 7)
        textureKey = "lotDonuts";
    drawSprite(textureKey, _tileSize * 0.50f, x, y, 1.3f, 1.3f);
}

void Sfml::drawOres(int x, int y, const TileData_t &tile)
{
    if (_tileSize <= 0.0f || tile.ressources.empty())
        return;
    int total_ores = tile.ressources[1] + tile.ressources[2] + tile.ressources[3] + tile.ressources[4] + tile.ressources[5] + tile.ressources[6];
    if (total_ores <= 0)
        return;
    std::string textureKey = "fewOres";
    if (total_ores >= 3 && total_ores < 7)
        textureKey = "mediumOres";
    else if (total_ores >= 7)
        textureKey = "lotOres";
    drawSprite(textureKey, _tileSize * 0.50f, x, y, 1.3f, 10.0f);
}

void Sfml::drawEggs(int x, int y, const TileData_t &tile)
{
    if (_tileSize <= 0.0f || tile.eggs.empty() || tile.eggs.size() <= 0)
        return;
    std::string textureKey = "egg";
    if (tile.eggs.size() == 2)
        textureKey = "twoeggs";
    else if (tile.eggs.size() >= 3)
        textureKey = "threeEggs";
    auto tex = _textures.find(textureKey);
    if (tex == _textures.end()) {
        return;
    }
    drawSprite(textureKey, _tileSize * 0.75f, x, y, 4.0f, 6.0f);
}

void Sfml::drawTrantorians(const TileData_t &tile)
{
    float desiredTrantorianSize = _tileSize * 0.95f; 
    auto tex = _textures.find("benoit");
    
    if (tex == _textures.end())
        return;
    const sf::Texture &texture = tex->second;
    sf::Sprite &playerSprite = _sprites["benoit"];
    playerSprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    playerSprite.setScale(desiredTrantorianSize / texture.getSize().x, desiredTrantorianSize / texture.getSize().y);
    for (const auto &animPair : _playerAnims) {
        const PlayerAnim_t &anim = animPair.second;
        auto playerInTileIt = tile.players.find(anim.id);
        bool isPlayerInTile = (playerInTileIt != tile.players.end());
        if (isPlayerInTile || anim.isDying) {
            if (anim.isDying) {
                auto deathTexIt = _textures.find("spritesheetDeath");
                if (deathTexIt != _textures.end()) {
                    sf::Sprite &deathSprite = _sprites["spritesheetDeath"];
                    const sf::Texture &deathTex = deathTexIt->second;
                    int totalFrames = 32; 
                    int frameWidth = deathTex.getSize().x / totalFrames;
                    int frameHeight = deathTex.getSize().y;
                    deathSprite.setTextureRect(sf::IntRect(anim.deathFrame * frameWidth, 0, frameWidth, frameHeight));
                    deathSprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
                    deathSprite.setScale(desiredTrantorianSize / frameWidth, desiredTrantorianSize / frameHeight);
                    deathSprite.setPosition(anim.visualPos);
                    deathSprite.setRotation(0.0f);
                    _window.draw(deathSprite);
                }
            } else {
                playerSprite.setPosition(anim.visualPos);
                if (anim.isMoving) {
                    float angle = 15.0f * std::sin(anim.walkTimer * 18.0f);
                    playerSprite.setRotation(angle);
                } else {
                    playerSprite.setRotation(0.0f);
                }
                _window.draw(playerSprite);                
                displayBubble(anim);
            }
        }
    }
    playerSprite.setRotation(0.0f);
}

void Sfml::displayBubble(const PlayerAnim_t &anim)
{
    auto bubbleTexIt = _textures.find("bubble");
    bool hasBubbleBg = (bubbleTexIt != _textures.end());

    if (!anim.bubbleQueue.empty() && hasBubbleBg) {
        auto iconTexIt = _textures.find(anim.bubbleQueue.front().first);                    
        if (iconTexIt != _textures.end()) {
            sf::Sprite &bubbleSprite = _sprites["bubble"];
            sf::Sprite &iconSprite = _sprites[anim.bubbleQueue.front().first];
            float desiredBubbleSize = _tileSize * 0.60f;
            float desiredIconSize = _tileSize * 0.40f;
            const sf::Texture &bubbleTex = bubbleTexIt->second;
            bubbleSprite.setOrigin(bubbleTex.getSize().x / 2.0f, bubbleTex.getSize().y / 2.0f);
            bubbleSprite.setScale(desiredBubbleSize / bubbleTex.getSize().x, desiredBubbleSize / bubbleTex.getSize().y);
            float bubbleX = anim.visualPos.x;
            float bubbleY = anim.visualPos.y - ((_tileSize * 0.95f) / 2.0f) - (desiredBubbleSize / 2.0f) - 4.0f;
            bubbleSprite.setPosition(bubbleX, bubbleY);                        
            const sf::Texture &iconTex = iconTexIt->second;
            iconSprite.setOrigin(iconTex.getSize().x / 2.0f, iconTex.getSize().y / 2.0f);
            iconSprite.setScale(desiredIconSize / iconTex.getSize().x, desiredIconSize / iconTex.getSize().y);
            iconSprite.setPosition(bubbleX, bubbleY);                        
            _window.draw(bubbleSprite);
            _window.draw(iconSprite);
        }
    } 
}

void Sfml::drawTileElements(int x, int y, const TileData_t &tile)
{
    drawFood(x, y, tile);
    drawOres(x, y, tile);
    drawEggs(x, y, tile);
    drawTrantorians(tile);
}

void Sfml::drawBackground()
{
    auto tex = _textures.find("background");
    if (tex == _textures.end())
        return;

    const sf::Texture &texture = tex->second;
    sf::Sprite &bgSprite = _sprites["background"];
    bgSprite.setPosition(0.0f, 0.0f);
    float scaleX = static_cast<float>(_window.getSize().x) / texture.getSize().x;
    float scaleY = static_cast<float>(_window.getSize().y) / texture.getSize().y;
    bgSprite.setScale(scaleX, scaleY);
    _window.draw(bgSprite);
}

void Sfml::displayWindow()
{
    float deltaTime = _clock.restart().asSeconds();

    if (_tileSize == 0.0f && _world.getMapSize().first != 0)
        updateDimensions();
    updateAnimations(deltaTime);

    _window.clear(sf::Color::Black);
    _window.setView(_camera); //c'est pour afficher les éléments
    drawBackground();
    drawMap();

    //_window.setView(_window.getDefaultView()); // et ça c'est pour le UI
    _window.display();
}

void Sfml::updateDimensions()
{
    if (_world.getMapSize().first == 0 || _world.getMapSize().second == 0)
        return;
    float currentWindowWidth = static_cast<float>(_window.getSize().x);
    float currentWindowHeight = static_cast<float>(_window.getSize().y);
    float mapAreaHeight = currentWindowHeight * 0.80f;
    _tileSize = mapAreaHeight / _world.getMapSize().second;
    float finalMapWidth = _tileSize * _world.getMapSize().first;
    _offsetY = (currentWindowHeight - mapAreaHeight) / 2.0f;
    _offsetX = (currentWindowWidth - finalMapWidth) / 2.0f;
    _limitWindowWidth = currentWindowWidth;
    _camera.setSize(currentWindowWidth, currentWindowHeight);
    _camera.setCenter(currentWindowWidth / 2.0f, currentWindowHeight / 2.0f);
}

int Sfml::loadTexture()
{
    std::string path = "gui/assets/images";

    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (!entry.is_regular_file())
                continue;
            sf::Texture texture;
            sf::Sprite sprite;
            if (!texture.loadFromFile(entry.path().string())) {
                std::cerr << "fail to load " << entry.path().string() << std::endl;
                continue;
            }
            _textures[entry.path().stem().string()] = std::move(texture);
            sprite.setTexture(_textures[entry.path().stem().string()]);
            _sprites[entry.path().stem().string()] = std::move(sprite);
            std::cout << "texture loaded : " << entry.path().stem().string() << std::endl;
        }
    } else {
        throw GuiException("file assets dosen't exist");
        return 84;
    }
    return 0;
}


sf::Vector2f Sfml::convertToPixels(int x, int y) const
{
    float centerX = _offsetX + (x * _tileSize) + (_tileSize / 2.0f);
    float centerY = _offsetY + (y * _tileSize) + (_tileSize / 2.0f);
    return sf::Vector2f(centerX, centerY);
}

void Sfml::setPlayerActionBubble(int id, const std::string &textureKey, float duration)
{
    PlayerAnim_t &anim = _playerAnims[id];
    
    anim.bubbleQueue.push_back({textureKey, duration});
    if (anim.bubbleTimer <= 0.0f)
        anim.bubbleTimer = duration;
}

void Sfml::updatePlayerAnimation(PlayerAnim_t &anim, const Player_t &player, float deltaTime, float maxWidth, float maxHeight)
{
    sf::Vector2f currentTilePixelPos = convertToPixels(player.x, player.y);

    if (anim.isDying) {
        updatePlayerDeath(anim, deltaTime);
        return;
    }
    if (anim.visualPos == sf::Vector2f(0.0f, 0.0f) && currentTilePixelPos != sf::Vector2f(0.0f, 0.0f))
        initPlayerAnim(anim, player, currentTilePixelPos);
    if (player.x != anim.lastX || player.y != anim.lastY) {
        anim.targetPos = currentTilePixelPos;
        anim.isMoving = true;
        anim.lastX = player.x;
        anim.lastY = player.y;
    }
    updatePlayerPosition(anim, deltaTime, maxWidth, maxHeight);
    updatePlayerBubble(anim, deltaTime);
}

void Sfml::initPlayerAnim(PlayerAnim_t &anim, const Player_t &player, const sf::Vector2f &pixelPos)
{
    anim.id = player.id;
    anim.visualPos = pixelPos;
    anim.targetPos = pixelPos;
    anim.lastX = player.x;
    anim.lastY = player.y;
    anim.isMoving = false;
}

void Sfml::updatePlayerPosition(PlayerAnim_t &anim, float deltaTime, float maxWidth, float maxHeight)
{
    if (!anim.isMoving) {
        anim.walkTimer = 0.0f;
        return;
    }
    if (std::abs(anim.targetPos.x - anim.visualPos.x) > maxWidth / 2.0f || 
        std::abs(anim.targetPos.y - anim.visualPos.y) > maxHeight / 2.0f) {
        anim.visualPos = anim.targetPos;
        anim.isMoving = false;
        anim.walkTimer = 0.0f;
        return;
    }
    float dx = anim.targetPos.x - anim.visualPos.x;
    float dy = anim.targetPos.y - anim.visualPos.y;
    if (std::abs(dx) < 1.0f && std::abs(dy) < 1.0f) {
        anim.visualPos = anim.targetPos;
        anim.isMoving = false;
        anim.walkTimer = 0.0f;
    } else {
        anim.visualPos.x += dx * 12.0f * deltaTime;
        anim.visualPos.y += dy * 12.0f * deltaTime;
        anim.walkTimer += deltaTime;
    }
}

void Sfml::updatePlayerBubble(PlayerAnim_t &anim, float deltaTime)
{
    if (anim.bubbleTimer <= 0.0f)
        return;
    anim.bubbleTimer -= deltaTime;
    if (anim.bubbleTimer <= 0.0f && !anim.bubbleQueue.empty()) {
        anim.bubbleQueue.erase(anim.bubbleQueue.begin());
        if (!anim.bubbleQueue.empty()) {
            anim.bubbleTimer = anim.bubbleQueue.front().second;
        }
    }
}

void Sfml::updateAnimations(float deltaTime)
{
    float maxMapWidthPixels = _world.getMapSize().first * _tileSize;
    float maxMapHeightPixels = _world.getMapSize().second * _tileSize;

    for (size_t y = 0; y < _world.getMapSize().second; ++y) {
        for (size_t x = 0; x < _world.getMapSize().first; ++x) {
            TileData_t &tile = _world.getTileData(x, y);
            for (const auto &pair : tile.players) {
                const Player_t &player = pair.second;
                PlayerAnim_t &anim = _playerAnims[player.id];
                updatePlayerAnimation(anim, player, deltaTime, maxMapWidthPixels, maxMapHeightPixels);
            }
        }
    }
    for (auto it = _playerAnims.begin(); it != _playerAnims.end();) {
        if (it->second.isDeadAndGone) {
            _world.removeTrantorian(it->second.id);
            it = _playerAnims.erase(it);
        } else
            ++it;
    }
}

void Sfml::triggerPlayerDeath(int id)
{
    if (_playerAnims.find(id) != _playerAnims.end()) {
        _playerAnims[id].isDying = true;
        _playerAnims[id].deathFrame = 0;
        _playerAnims[id].deathTimer = 0.0f;
    }
}

void Sfml::updatePlayerDeath(PlayerAnim_t &anim, float deltaTime)
{
    const int totalFrames = 32;
    const float frameDuration = 0.1f;


    if (!anim.isDying)
        return;
    anim.deathTimer += deltaTime;
    if (anim.deathTimer >= frameDuration) {
        anim.deathTimer = 0.0f;
        anim.deathFrame++;
        if (anim.deathFrame >= totalFrames) {
            anim.isDeadAndGone = true;
        }
    }
}