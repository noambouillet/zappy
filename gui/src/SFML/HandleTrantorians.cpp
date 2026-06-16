/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** HandleTrantorians
*/

#include "HandleTrantorians.hpp"
#include <math.h>

HandleTrantorians::HandleTrantorians(TextureManager &textureManager, sf::RenderWindow  &window, World &world): _textureManager(textureManager), _window(window), _world(world) {}

HandleTrantorians::~HandleTrantorians() {}

sf::Vector2f HandleTrantorians::convertToPixels(int x, int y) const
{
    float centerX = _offsetX + (x * _tileSize) + (_tileSize / 2.0f);
    float centerY = _offsetY + (y * _tileSize) + (_tileSize / 2.0f);
    return sf::Vector2f(centerX, centerY);
}

void HandleTrantorians::setPlayerActionBubble(int id, const std::string &textureKey, float duration)
{
    PlayerAnim_t &anim = _playerAnims[id];
    
    anim.bubbleQueue.push_back({textureKey, duration});
    if (anim.bubbleTimer <= 0.0f)
        anim.bubbleTimer = duration;
}

void HandleTrantorians::updatePlayerAnimation(PlayerAnim_t &anim, const Player_t &player, float deltaTime, float maxWidth, float maxHeight)
{
    sf::Vector2f currentTilePixelPos = convertToPixels(player.x, player.y);

    if (anim.isDying) {
        updatePlayerDeath(anim, deltaTime);
        return;
    }
    if (anim.isIncanting) {
        updatePlayerIncantation(anim, deltaTime);
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

void HandleTrantorians::initPlayerAnim(PlayerAnim_t &anim, const Player_t &player, const sf::Vector2f &pixelPos)
{
    anim.id = player.id;
    anim.visualPos = pixelPos;
    anim.targetPos = pixelPos;
    anim.lastX = player.x;
    anim.lastY = player.y;
    anim.isMoving = false;
}

void HandleTrantorians::updatePlayerPosition(PlayerAnim_t &anim, float deltaTime, float maxWidth, float maxHeight)
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

void HandleTrantorians::updatePlayerBubble(PlayerAnim_t &anim, float deltaTime)
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

void HandleTrantorians::updateAnimations(float deltaTime)
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

void HandleTrantorians::triggerPlayerDeath(int id)
{
    if (_playerAnims.find(id) != _playerAnims.end()) {
        _playerAnims[id].isDying = true;
        _playerAnims[id].deathFrame = 0;
        _playerAnims[id].deathTimer = 0.0f;
    }
}

void HandleTrantorians::updatePlayerDeath(PlayerAnim_t &anim, float deltaTime)
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

void HandleTrantorians::setPlayerIncanting(int id, bool state)
{
    if (_playerAnims.find(id) != _playerAnims.end()) {
        _playerAnims[id].isIncanting = state;
        _playerAnims[id].incantFrame = 0;
        _playerAnims[id].incantTimer = 0.0f;
    }
}

void HandleTrantorians::stopIncantationAt(int x, int y)
{
    for (auto &pair : _playerAnims) {
        if (pair.second.lastX == x && pair.second.lastY == y) {
            pair.second.isIncanting = false;
        }
    }
}

void HandleTrantorians::updatePlayerIncantation(PlayerAnim_t &anim, float deltaTime)
{
    const int totalFrames = 32;
    const float frameDuration = 0.05f;

    if (!anim.isIncanting)
        return;
    anim.incantTimer += deltaTime;
    if (anim.incantTimer >= frameDuration) {
        anim.incantTimer = 0.0f;
        anim.incantFrame = (anim.incantFrame + 1) % totalFrames; 
    }
}

void HandleTrantorians::drawTrantorians(const TileData_t &tile)
{
    float trantorianSize = _tileSize * 0.95f; 
    const sf::Texture &texture = _textureManager.getTexture("benoit");
    sf::Sprite &playerSprite = _textureManager.getSprite("benoit");

    playerSprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    playerSprite.setScale(trantorianSize / texture.getSize().x, trantorianSize / texture.getSize().y);
    for (const auto &animPair : _playerAnims) {
        const PlayerAnim_t &anim = animPair.second;
        auto playerInTileIt = tile.players.find(anim.id);
        bool isPlayerInTile = (playerInTileIt != tile.players.end());
        if (isPlayerInTile || anim.isDying) {
            if (anim.isDying)
                drawAnimation(anim, trantorianSize, 32,"spritesheetDeath");
            else if (anim.isIncanting)
                drawAnimation(anim, trantorianSize, 6, "incantation");
            else {
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

void HandleTrantorians::drawAnimation(const PlayerAnim_t &anim, float size, int NbFrame, std::string spritesheet)
{
    const sf::Texture &texture = _textureManager.getTexture(spritesheet);
    sf::Sprite &sprite = _textureManager.getSprite(spritesheet);
    int totalFrames = NbFrame; 
    int frameWidth = texture.getSize().x / totalFrames;
    int frameHeight = texture.getSize().y;
    sprite.setTextureRect(sf::IntRect(anim.deathFrame * frameWidth, 0, frameWidth, frameHeight)); // problème à régler c'est sur deathframe
    sprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
    sprite.setScale(size / frameWidth, size / frameHeight);
    sprite.setPosition(anim.visualPos);
    sprite.setRotation(0.0f);
    _window.draw(sprite);
}

void HandleTrantorians::displayBubble(const PlayerAnim_t &anim)
{
    float BubbleSize = _tileSize * 0.60f;
    float IconSize = _tileSize * 0.40f;
    const sf::Texture &bubbleTexture = _textureManager.getTexture("bubble");
    sf::Sprite &bubbleSprite = _textureManager.getSprite("bubble");

    if (!anim.bubbleQueue.empty()) {
        const sf::Texture &iconTexture = _textureManager.getTexture(anim.bubbleQueue.front().first);
        sf::Sprite &iconSprite = _textureManager.getSprite(anim.bubbleQueue.front().first);
        bubbleSprite.setOrigin(bubbleTexture.getSize().x / 2.0f, bubbleTexture.getSize().y / 2.0f);
        bubbleSprite.setScale(BubbleSize / bubbleTexture.getSize().x, BubbleSize / bubbleTexture.getSize().y);
        float bubbleX = anim.visualPos.x;
        float bubbleY = anim.visualPos.y - ((_tileSize * 0.95f) / 2.0f) - (BubbleSize / 2.0f) - 4.0f;
        bubbleSprite.setPosition(bubbleX, bubbleY);                        
        iconSprite.setOrigin(iconTexture.getSize().x / 2.0f, iconTexture.getSize().y / 2.0f);
        iconSprite.setScale(IconSize / iconTexture.getSize().x, IconSize / iconTexture.getSize().y);
        iconSprite.setPosition(bubbleX, bubbleY);                        
        _window.draw(bubbleSprite);
        _window.draw(iconSprite);
    } 
}

void HandleTrantorians::update(float tileSize, float offsetX, float offsetY)
{
    _tileSize = tileSize;
    _offsetX = offsetX;
    _offsetY = offsetY;
}