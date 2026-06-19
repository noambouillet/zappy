/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** HandleTrantorians
*/

#include "HandleTrantorians.hpp"
#include <math.h>

HandleTrantorians::HandleTrantorians(TextureManager &textureManager, sf::RenderWindow &window, World &world): _textureManager(textureManager), _window(window), _world(world) {}

HandleTrantorians::~HandleTrantorians() {}

sf::Vector2f HandleTrantorians::convertToPixels(int x, int y) const
{
    return sf::Vector2f(_offsetX + (x * _tileSize) + (_tileSize / 2.0f), _offsetY + (y * _tileSize) + (_tileSize / 2.0f));
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
    if (anim.isDying) {
        updatePlayerDeath(anim, deltaTime);
        return;
    }
    if (anim.isIncanting)
        updatePlayerIncantation(anim, deltaTime);
    sf::Vector2f currentTilePixelPos = convertToPixels(player.x, player.y);
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
        if (!anim.bubbleQueue.empty())
            anim.bubbleTimer = anim.bubbleQueue.front().second;
    }
}

void HandleTrantorians::updateAnimations(float deltaTime)
{
    for (size_t y = 0; y < _world.getMapSize().second; ++y) {
        for (size_t x = 0; x < _world.getMapSize().first; ++x) {
            TileData_t &tile = _world.getTileData(x, y);
            for (const auto &pair : tile.players) {
                updatePlayerAnimation(_playerAnims[pair.second.id], pair.second, deltaTime, _world.getMapSize().first * _tileSize, _world.getMapSize().second * _tileSize);
            }
        }
    }
    for (auto it = _playerAnims.begin(); it != _playerAnims.end();) {
        if (it->second.isDeadAndGone) {
            _world.removeTrantorian(it->second.id);
            it = _playerAnims.erase(it);
        } else {
            it++;
        }
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
    if (!anim.isDying)
        return;
    anim.deathTimer += deltaTime;
    if (anim.deathTimer >= 0.1f) {
        anim.deathTimer = 0.0f;
        anim.deathFrame++;
        if (anim.deathFrame >= 32)
            anim.isDeadAndGone = true;
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
    for (auto &pair : _playerAnims)
        if (pair.second.lastX == x && pair.second.lastY == y)
            pair.second.isIncanting = false;
}

void HandleTrantorians::updatePlayerIncantation(PlayerAnim_t &anim, float deltaTime)
{
    if (!anim.isIncanting)
        return;
    anim.incantTimer += deltaTime;
    if (anim.incantTimer >= 0.05f) {
        anim.incantTimer = 0.0f;
        anim.incantFrame = (anim.incantFrame + 1) % 32;
    }
}

void HandleTrantorians::drawBaseIndicator(const sf::Vector2f &pos)
{
    sf::CircleShape baseIndicator(_tileSize * 0.4f);
    baseIndicator.setOrigin(baseIndicator.getRadius(), baseIndicator.getRadius());
    baseIndicator.setPosition(pos.x, pos.y + _tileSize * 0.2f);
    baseIndicator.setFillColor(sf::Color(178, 102, 180));
    baseIndicator.setOutlineColor(sf::Color(76, 0, 153));
    baseIndicator.setOutlineThickness(1.5f);
    _window.draw(baseIndicator);
}

void HandleTrantorians::drawTrantorians(const TileData_t &tile)
{
    float size = _tileSize * 0.95f;
    const sf::Texture &tex = _textureManager.getTexture("benoit");
    sf::Sprite &sprite = _textureManager.getSprite("benoit");
    sprite.setOrigin(tex.getSize().x / 2.0f, tex.getSize().y / 2.0f);
    std::string activeTeam = _world.getSelectedTeam();
    int hoveredPlayerId = _world.getSelectedPlayerId();
    for (const auto &animPair : _playerAnims) {
        const PlayerAnim_t &anim = animPair.second;
        auto it = tile.players.find(anim.id);
        bool hasPlayer = (it != tile.players.end());
        if (hasPlayer || anim.isDying) {
            bool isTargetTeam = (!activeTeam.empty() && hasPlayer && it->second.teamName == activeTeam);
            if (isTargetTeam || (hasPlayer && hoveredPlayerId == it->second.id))
                drawBaseIndicator(anim.visualPos);
            if (activeTeam.empty())
                sprite.setScale(size / tex.getSize().x, size / tex.getSize().y);
            if (anim.isDying) {
                drawAnimation(anim, size, 32, "spritesheetDeath", anim.deathFrame);
            } else if (anim.isIncanting) {
                drawAnimation(anim, size, 6, "incantation", anim.incantFrame);
            } else {
                sprite.setPosition(anim.visualPos);
                sprite.setRotation((anim.isMoving) ? (15.0f * std::sin(anim.walkTimer * 18.0f)) : 0.0f);
                _window.draw(sprite);
                displayBubble(anim);
            }
        }
    }
    sprite.setRotation(0.0f);
    sprite.setColor(sf::Color::White);
}

void HandleTrantorians::drawAnimation(const PlayerAnim_t &anim, float size, int nbFrame, std::string sheetKey, int currentFrame)
{
    const sf::Texture &tex = _textureManager.getTexture(sheetKey);
    sf::Sprite &sprite = _textureManager.getSprite(sheetKey);
    int frameW = tex.getSize().x / nbFrame;
    int frameH = tex.getSize().y;

    sprite.setTextureRect(sf::IntRect(currentFrame * frameW, 0, frameW, frameH));
    sprite.setOrigin(frameW / 2.0f, frameH / 2.0f);
    sprite.setScale(size / frameW, size / frameH);
    sprite.setPosition(anim.visualPos);
    sprite.setRotation(0.0f);
    _window.draw(sprite);
}

void HandleTrantorians::displayBubble(const PlayerAnim_t &anim)
{
    if (anim.bubbleQueue.empty())
        return;
    float bSize = _tileSize * 0.60f;
    float iSize = _tileSize * 0.40f;
    const sf::Texture &bTex = _textureManager.getTexture("bubble");
    sf::Sprite &bSprite = _textureManager.getSprite("bubble");
    const sf::Texture &iTex = _textureManager.getTexture(anim.bubbleQueue.front().first);
    sf::Sprite &iSprite = _textureManager.getSprite(anim.bubbleQueue.front().first);

    bSprite.setOrigin(bTex.getSize().x / 2.0f, bTex.getSize().y / 2.0f);
    bSprite.setScale(bSize / bTex.getSize().x, bSize / bTex.getSize().y);
    float bX = anim.visualPos.x;
    float bY = anim.visualPos.y - ((_tileSize * 0.95f) / 2.0f) - (bSize / 2.0f) - 4.0f;
    bSprite.setPosition(bX, bY);
    iSprite.setOrigin(iTex.getSize().x / 2.0f, iTex.getSize().y / 2.0f);
    iSprite.setScale(iSize / iTex.getSize().x, iSize / iTex.getSize().y);
    iSprite.setPosition(bX, bY);
    _window.draw(bSprite);
    _window.draw(iSprite);
}

void HandleTrantorians::update(float tileSize, float offsetX, float offsetY)
{
    _tileSize = tileSize;
    _offsetX = offsetX;
    _offsetY = offsetY;
}