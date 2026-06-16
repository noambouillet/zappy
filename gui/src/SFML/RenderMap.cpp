/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RenderMap
*/

#include "RenderMap.hpp"

RenderMap::RenderMap(TextureManager &textureManager, sf::RenderWindow  &window, World &world): _textureManager(textureManager), _window(window), _world(world), _handleTrantorians(_textureManager, _window, _world) {}

RenderMap::~RenderMap() {}

void RenderMap::drawFood(int x, int y, const TileData_t &tile)
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

void RenderMap::drawOres(int x, int y, const TileData_t &tile)
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

void RenderMap::drawEggs(int x, int y, const TileData_t &tile)
{
    if (_tileSize <= 0.0f || tile.eggs.empty() || tile.eggs.size() <= 0)
        return;
    std::string textureKey = "egg";
    if (tile.eggs.size() == 2)
        textureKey = "twoeggs";
    else if (tile.eggs.size() >= 3)
        textureKey = "threeEggs";
    drawSprite(textureKey, _tileSize * 0.75f, x, y, 4.0f, 6.0f);
}

void RenderMap::drawMap() 
{
    sf::RectangleShape tileShape(sf::Vector2f(_tileSize, _tileSize));
    tileShape.setOutlineColor(sf::Color::Black);
    tileShape.setOutlineThickness(-1.0f);

    const sf::Texture &tex = _textureManager.getTexture("ground");
    tileShape.setTexture(&tex);
    tileShape.setFillColor(sf::Color::White);
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

void RenderMap::drawTileElements(int x, int y, const TileData_t &tile)
{
    drawFood(x, y, tile);
    drawOres(x, y, tile);
    drawEggs(x, y, tile);
    _handleTrantorians.drawTrantorians(tile);
}

void RenderMap::drawBackground()
{
    const sf::Texture &texture = _textureManager.getTexture("background");
    sf::Sprite &sprite = _textureManager.getSprite("background");
    sprite.setPosition(0.0f, 0.0f);
    float scaleX = static_cast<float>(_window.getSize().x) / texture.getSize().x;
    float scaleY = static_cast<float>(_window.getSize().y) / texture.getSize().y;
    sprite.setScale(scaleX, scaleY);
    _window.draw(sprite);
}

void RenderMap::drawSprite(std::string textureKey, float size, int x, int y, float offsetX, float offsetY)
{
    const sf::Texture &texture = _textureManager.getTexture(textureKey);
    sf::Sprite &sprite = _textureManager.getSprite(textureKey);
    float centerX = _offsetX + (x * _tileSize) + (_tileSize / offsetX);
    float centerY = _offsetY + (y * _tileSize) + (_tileSize / offsetY);
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    sprite.setScale(size / texture.getSize().x, size / texture.getSize().y);
    sprite.setPosition(centerX, centerY);
    _window.draw(sprite);
}

void RenderMap::update(float tileSize, float offsetX, float offsetY)
{
    _tileSize = tileSize;
    _offsetX = offsetX;
    _offsetY = offsetY;
    _handleTrantorians.update(tileSize, offsetX, offsetY);
}

HandleTrantorians &RenderMap::getHandleTrantorians()
{
    return _handleTrantorians;
}
