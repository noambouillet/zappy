/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** Sfml
*/

#include "Sfml.hpp"

Sfml::Sfml(): _window(sf::VideoMode::getDesktopMode(), "trantor", sf::Style::Titlebar | sf::Style::Close)
{
    _window.setFramerateLimit(60);

    if (!_foodTexture.loadFromFile("gui/assets/images/donut.png")) {
        // throw une erreur: texture donut not found
    }
    _foodSprite.setTexture(_foodTexture);
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
    sf::Event event;
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            _window.close();
        }
}

void Sfml::drawMap() 
{
    sf::RectangleShape tile(sf::Vector2f(_tileSize, _tileSize));
    tile.setFillColor(sf::Color(34, 139, 34));
    tile.setOutlineColor(sf::Color::Black);
    tile.setOutlineThickness(-1.0f);

    for (size_t y = 0; y < _mapSize.second; ++y) {
        for (size_t x = 0; x < _mapSize.first; ++x) {
            tile.setPosition(_offsetX + (x * _tileSize), _offsetY + (y * _tileSize));
            _window.draw(tile);
        }
    }
}

void Sfml::drawRessources()
{
    float originX = _foodTexture.getSize().x / 2.0f;
    float originY = _foodTexture.getSize().y / 2.0f;
    _foodSprite.setOrigin(originX, originY);

    float desiredFoodSize = _tileSize * 0.40f; 
    float scaleX = desiredFoodSize / _foodTexture.getSize().x;
    float scaleY = desiredFoodSize / _foodTexture.getSize().y;
    _foodSprite.setScale(scaleX, scaleY);

    for (size_t y = 0; y < _mapSize.second; ++y) {
        for (size_t x = 0; x < _mapSize.first; ++x) {
            if (!_map[y][x].empty() && _map[y][x][0] > 0) {
                float tileX = _offsetX + (x * _tileSize);
                float tileY = _offsetY + (y * _tileSize);
                float centerX = tileX + (_tileSize / 2.0f);
                float centerY = tileY + (_tileSize / 2.0f);
                _foodSprite.setPosition(centerX, centerY);
                _window.draw(_foodSprite);
            }
        }
    }
}

void Sfml::setTile(int x, int y, std::vector<int> ressources)
{
    _map[x][y] = ressources;
}

void Sfml::setTimeUnit(int TimeUnit)
{
    _TimeUnit = TimeUnit;
}

void Sfml::displayWindow()
{
    _window.clear(sf::Color::Black);
    drawMap();
    drawRessources();
    _window.display();
}

void Sfml::setMapSize(size_t width, size_t height)
{
    _mapSize = {width, height};
    _map.resize(height);
    for (size_t y = 0; y < height; ++y) {
        _map[y].resize(width);
        for (size_t x = 0; x < width; ++x) {
            _map[y][x].resize(7, 0);
        }
    }

    updateDimensions(); 
}

void Sfml::updateDimensions()
{
    if (_mapSize.first == 0 || _mapSize.second == 0)
        return;

    float currentWindowWidth = static_cast<float>(_window.getSize().x);
    float currentWindowHeight = static_cast<float>(_window.getSize().y);
    float mapAreaHeight = currentWindowHeight * 0.80f;
    _tileSize = mapAreaHeight / _mapSize.second;

    float finalMapWidth = _tileSize * _mapSize.first;
    _offsetY = (currentWindowHeight - mapAreaHeight) / 2.0f;
    _offsetX = (currentWindowWidth - finalMapWidth) / 2.0f;
}