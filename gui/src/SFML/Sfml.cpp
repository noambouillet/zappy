/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** Sfml
*/

#include "Sfml.hpp"

Sfml::Sfml(const World &world): _window(sf::VideoMode::getDesktopMode(), "trantor", sf::Style::Titlebar | sf::Style::Close), _world(world)
{
    _window.setFramerateLimit(60);

    if (!_foodTexture.loadFromFile("gui/assets/images/donut.png") || !_eggTexture.loadFromFile("gui/assets/images/egg.jpg") || !_trantorianTexture.loadFromFile("gui/assets/images/benoit.png")) {
        // throw une erreur: texture donut not found
    }
    _foodSprite.setTexture(_foodTexture);
    _eggSprite.setTexture(_eggTexture);
    _trantorianSprite.setTexture(_trantorianTexture);
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

    for (size_t y = 0; y < _world.getMapSize().second; ++y) {
        for (size_t x = 0; x < _world.getMapSize().first; ++x) {
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

    for (size_t y = 0; y < _world.getMapSize().second; ++y) {
        for (size_t x = 0; x < _world.getMapSize().first; ++x) {
            if (!_world.getMap()[y][x].empty() && _world.getMap()[y][x][0] > 0) {
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

void Sfml::drawEggs()
{
    if (_eggTexture.getSize().x == 0 || _world.getEggs().empty())
        return;

    float originX = _eggTexture.getSize().x / 2.0f;
    float originY = _eggTexture.getSize().y / 2.0f;
    _eggSprite.setOrigin(originX, originY);

    float desiredeggSize = _tileSize * 0.40f; 
    float scaleX = desiredeggSize / _eggTexture.getSize().x;
    float scaleY = desiredeggSize / _eggTexture.getSize().y;
    _eggSprite.setScale(scaleX, scaleY);

    for (const auto &egg : _world.getEggs()) {
        if (egg.size() < 4)
            continue;
        int x = egg[2];
        int y = egg[3];
        float tileX = _offsetX + (x * _tileSize);
        float tileY = _offsetY + (y * _tileSize);
        float centerX = tileX + (_tileSize / 2.0f);
        float centerY = tileY + (_tileSize / 2.0f);

        _eggSprite.setPosition(centerX, centerY);
        _window.draw(_eggSprite);
    }
}

void Sfml::drawTrantorians()
{
    if (_trantorianTexture.getSize().x == 0 || _world.getTrantorians().empty())
        return;

    float originX = _trantorianTexture.getSize().x / 2.0f;
    float originY = _trantorianTexture.getSize().y / 2.0f;
    _trantorianSprite.setOrigin(originX, originY);

    float desiredtrantorianSize = _tileSize * 0.95f; 
    float scaleX = desiredtrantorianSize / _trantorianTexture.getSize().x;
    float scaleY = desiredtrantorianSize / _trantorianTexture.getSize().y;
    _trantorianSprite.setScale(scaleX, scaleY);

    for (const auto &trantorian : _world.getTrantorians()) {
        int x = trantorian.second.x;
        int y = trantorian.second.y;
        float tileX = _offsetX + (x * _tileSize);
        float tileY = _offsetY + (y * _tileSize);
        float centerX = tileX + (_tileSize / 2.0f);
        float centerY = tileY + (_tileSize / 2.0f);

        _trantorianSprite.setPosition(centerX, centerY);
        _window.draw(_trantorianSprite);
    }
}

void Sfml::displayWindow()
{
    if (_tileSize == 0.0f && _world.getMapSize().first != 0)
        updateDimensions();
    _window.clear(sf::Color::Black);
    drawMap();
    drawRessources();
    drawEggs();
    drawTrantorians();
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
}
