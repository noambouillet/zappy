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

void HandleTrantorians::setTrantorianActionBubble(int id, const std::string &textureKey, float duration)
{
    TrantorianAnim_t &anim = _trantorianAnims[id];

    anim.bubbleQueue.push_back({textureKey, duration});
    if (anim.bubbleTimer <= 0.0f)
        anim.bubbleTimer = duration;
}

void HandleTrantorians::updateTrantorianAnimation(TrantorianAnim_t &anim, const Trantorian_t &trantorian, float deltaTime, float maxWidth, float maxHeight)
{
    if (anim.isDying) {
        updateTrantorianDeath(anim, deltaTime);
        return;
    }
    if (anim.isIncanting)
        updateTrantorianIncantation(anim, deltaTime);
    sf::Vector2f currentTilePixelPos = convertToPixels(trantorian.x, trantorian.y);
    if (anim.visualPos == sf::Vector2f(0.0f, 0.0f) && currentTilePixelPos != sf::Vector2f(0.0f, 0.0f))
        initTrantorianAnim(anim, trantorian, currentTilePixelPos);
    if (trantorian.x != anim.lastX || trantorian.y != anim.lastY) {
        anim.targetPos = currentTilePixelPos;
        anim.isMoving = true;
        anim.lastX = trantorian.x;
        anim.lastY = trantorian.y;
    }
    updateTrantorianPosition(anim, deltaTime, maxWidth, maxHeight);
    updateTrantorianBubble(anim, deltaTime);
}

void HandleTrantorians::initTrantorianAnim(TrantorianAnim_t &anim, const Trantorian_t &trantorian, const sf::Vector2f &pixelPos)
{
    anim.id = trantorian.id;
    anim.visualPos = pixelPos;
    anim.targetPos = pixelPos;
    anim.lastX = trantorian.x;
    anim.lastY = trantorian.y;
    anim.isMoving = false;
}

void HandleTrantorians::updateTrantorianPosition(TrantorianAnim_t &anim, float deltaTime, float maxWidth, float maxHeight)
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

void HandleTrantorians::updateTrantorianBubble(TrantorianAnim_t &anim, float deltaTime)
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
    for (size_t y = 0; y < _world.getMapSize().second; y++) {
        for (size_t x = 0; x < _world.getMapSize().first; x++) {
            TileData_t &tile = _world.getTileData(x, y);
            for (const auto &pair : tile.trantorians)
                updateTrantorianAnimation(_trantorianAnims[pair.second.id], pair.second, deltaTime, _world.getMapSize().first * _tileSize, _world.getMapSize().second * _tileSize);
        }
    }
    for (auto it = _trantorianAnims.begin(); it != _trantorianAnims.end();) {
        if (it->second.isDeadAndGone) {
            _world.removeTrantorian(it->second.id);
            it = _trantorianAnims.erase(it);
        } else
            it++;
    }
}

void HandleTrantorians::triggerTrantorianDeath(int id)
{
    if (_trantorianAnims.find(id) != _trantorianAnims.end()) {
        _trantorianAnims[id].isDying = true;
        _trantorianAnims[id].deathFrame = 0;
        _trantorianAnims[id].deathTimer = 0.0f;
    }
}

void HandleTrantorians::updateTrantorianDeath(TrantorianAnim_t &anim, float deltaTime)
{
    anim.deathTimer += deltaTime;
    if (anim.deathTimer >= 0.1f) {
        anim.deathTimer = 0.0f;
        anim.deathFrame++;
        if (anim.deathFrame >= 32)
            anim.isDeadAndGone = true;
    }
}

void HandleTrantorians::setTrantorianIncanting(int id, bool state)
{
    if (_trantorianAnims.find(id) != _trantorianAnims.end()) {
        _trantorianAnims[id].isIncanting = state;
        _trantorianAnims[id].incantFrame = 0;
        _trantorianAnims[id].incantTimer = 0.0f;
    }
}

void HandleTrantorians::stopIncantationAt(int x, int y)
{
    for (auto &pair : _trantorianAnims)
        if (pair.second.lastX == x && pair.second.lastY == y)
            pair.second.isIncanting = false;
}

void HandleTrantorians::updateTrantorianIncantation(TrantorianAnim_t &anim, float deltaTime)
{
    if (!anim.isIncanting)
        return;
    anim.incantTimer += deltaTime;
    if (anim.incantTimer >= 0.05f) {
        anim.incantTimer = 0.0f;
        anim.incantFrame = (anim.incantFrame + 1) % 4;
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

const sf::Color HandleTrantorians::getColorWithLvl(int lvl)
{
    static const sf::Color Colors[] = {sf::Color(255, 0, 0), sf::Color(255, 228, 0), sf::Color(165, 255, 0), sf::Color(0, 255, 106), sf::Color(0, 239, 255), sf::Color(0, 90, 255), sf::Color(69, 0, 255), sf::Color(228, 0, 255)};
    
    if (lvl < 1 || lvl > 8)
        return sf::Color::White; 
    return Colors[lvl - 1];
}

void HandleTrantorians::drawTrantorians(const TileData_t &tile)
{
    float size = _tileSize * 0.95f;
    const sf::Texture &tex = _textureManager.getTexture("trantorian");
    sf::Sprite &sprite = _textureManager.getSprite("trantorian");
    sprite.setOrigin(tex.getSize().x / 2.0f, tex.getSize().y / 2.0f);
    std::string activeTeam = _world.getSelectedTeam();
    int hoveredTrantorianId = _world.getSelectedTrantorianId();
    for (const auto &animPair : _trantorianAnims) {
        const TrantorianAnim_t &anim = animPair.second;
        auto it = tile.trantorians.find(anim.id);
        bool hasTrantorian = (it != tile.trantorians.end());
        if (hasTrantorian || anim.isDying) {
            bool isTargetTeam = (!activeTeam.empty() && hasTrantorian && it->second.teamName == activeTeam);
            if (isTargetTeam || (hasTrantorian && hoveredTrantorianId == it->second.id))
                drawBaseIndicator(anim.visualPos);
            float scaleX = size / tex.getSize().x;
            if (hasTrantorian) {
                if (it->second.orientation == 2)
                    scaleX = -scaleX;
            }
            sprite.setScale(scaleX, size / tex.getSize().y);
            if (_displayLvl)
                sprite.setColor(getColorWithLvl(it->second.level));
            if (anim.isDying)
                drawAnimation(anim, size * 1.5f, 32, "spritesheetDeath", anim.deathFrame);
            else if (anim.isIncanting)
                drawAnimation(anim, size, 6, "incantation", anim.incantFrame);
            else {
                sprite.setPosition(anim.visualPos);
                sprite.setRotation((anim.isMoving) ? (15.0f * std::sin(anim.walkTimer * 18.0f)) : 0.0f);
                _window.draw(sprite);
                displayBubble(anim);
            }
        }
    }
    sprite.setRotation(0.0f);
    sprite.setColor(sf::Color::White);
    if (_displayBroadcast)
        updateAndDrawWaves(_window);
}

void HandleTrantorians::drawAnimation(const TrantorianAnim_t &anim, float size, int nbFrame, std::string sheetKey, int currentFrame)
{
    const sf::Texture &tex = _textureManager.getTexture(sheetKey);
    sf::Sprite &sprite = _textureManager.getSprite(sheetKey);

    sprite.setTextureRect(sf::IntRect(currentFrame * tex.getSize().x / nbFrame, 0, tex.getSize().x / nbFrame, tex.getSize().y));
    sprite.setOrigin(tex.getSize().x / nbFrame / 2.0f, tex.getSize().y / 2.0f);
    sprite.setScale(size / (tex.getSize().x / nbFrame), size / tex.getSize().y);
    sprite.setPosition(anim.visualPos);
    sprite.setRotation(0.0f);
    _window.draw(sprite);
}

void HandleTrantorians::displayBubble(const TrantorianAnim_t &anim)
{
    if (anim.bubbleQueue.empty())
        return;
    float bubbleSize = _tileSize * 0.60f;
    float IconSize = _tileSize * 0.40f;
    const sf::Texture &bTex = _textureManager.getTexture("bubble");
    sf::Sprite &bSprite = _textureManager.getSprite("bubble");
    const sf::Texture &iTex = _textureManager.getTexture(anim.bubbleQueue.front().first);
    sf::Sprite &iSprite = _textureManager.getSprite(anim.bubbleQueue.front().first);

    bSprite.setOrigin(bTex.getSize().x / 2.0f, bTex.getSize().y / 2.0f);
    bSprite.setScale(bubbleSize / bTex.getSize().x, bubbleSize / bTex.getSize().y);
    bSprite.setPosition(anim.visualPos.x, anim.visualPos.y - ((_tileSize * 0.95f) / 2.0f) - (bubbleSize / 2.0f) - 4.0f);
    iSprite.setOrigin(iTex.getSize().x / 2.0f, iTex.getSize().y / 2.0f);
    iSprite.setScale(IconSize / iTex.getSize().x, IconSize / iTex.getSize().y);
    iSprite.setPosition(anim.visualPos.x, anim.visualPos.y - ((_tileSize * 0.95f) / 2.0f) - (bubbleSize / 2.0f) - 4.0f);
    _window.draw(bSprite);
    _window.draw(iSprite);
}

void HandleTrantorians::update(float tileSize, float offsetX, float offsetY)
{
    _tileSize = tileSize;
    _offsetX = offsetX;
    _offsetY = offsetY;
}

void HandleTrantorians::addBroadcast(int id)
{    
    _waves.erase(std::remove_if(_waves.begin(), _waves.end(),
        [id](const BroadcastWave &wave) {
            return wave.id == id;
        }), _waves.end());

    BroadcastWave wave;
    wave.id = id;
    _waves.push_back(wave);
}

void HandleTrantorians::updateAndDrawWaves(sf::RenderWindow &window)
{
    float deltaTime = _clock.restart().asSeconds();
    sf::CircleShape circle;
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(2.0f);

    for (auto it = _waves.begin(); it != _waves.end();) {
        it->radius += SPEED * deltaTime;
        if (it->radius >= MAX_RADIUS) {
            it = _waves.erase(it);
            continue;
        }
        if (_trantorianAnims.find(it->id) != _trantorianAnims.end()) {
            circle.setRadius(it->radius);
            circle.setOrigin(it->radius, it->radius);
            circle.setPosition(_trantorianAnims[it->id].visualPos);
            circle.setOutlineColor(sf::Color(151, 26, 251));
            window.draw(circle);
        }
        it++;
    }
}

void HandleTrantorians::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed) {
        if  (event.key.code == sf::Keyboard::B) {
            _displayBroadcast = !_displayBroadcast;
            _world.setDisplayBroadcast(_displayBroadcast);
        }
        if  (event.key.code == sf::Keyboard::L)  {
            _displayLvl = !_displayLvl;
            _world.setDisplayLvl(_displayLvl);
        }
    }
}