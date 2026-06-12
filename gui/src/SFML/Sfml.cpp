/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** Sfml
*/

#include "Sfml.hpp"
#include <algorithm>

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
    sf::RectangleShape tile(sf::Vector2f(_tileSize, _tileSize));
    
    auto tex = _textures.find("ground");
    
    if (tex != _textures.end()) {
        tile.setTexture(&tex->second);
        tile.setFillColor(sf::Color::White); 
    } else {
        tile.setFillColor(sf::Color(34, 139, 34));
    }
    tile.setOutlineColor(sf::Color::Black);
    tile.setOutlineThickness(-1.0f);
    for (size_t y = 0; y < _world.getMapSize().second; ++y) {
        for (size_t x = 0; x < _world.getMapSize().first; ++x) {
            tile.setPosition(_offsetX + (x * _tileSize), _offsetY + (y * _tileSize));
            _window.draw(tile);
        }
    }
}

void Sfml::drawBackground()
{
    auto tex = _textures.find("background");
    if (tex == _textures.end())
        return;

    const sf::Texture &texture = tex->second;
    sf::Sprite &bgSprite = _sprites["background"];
    bgSprite.setPosition(0.0f, 0.0f);
    float windowWidth = static_cast<float>(_window.getSize().x);
    float windowHeight = static_cast<float>(_window.getSize().y);
    float scaleX = windowWidth / texture.getSize().x;
    float scaleY = windowHeight / texture.getSize().y;
    bgSprite.setScale(scaleX, scaleY);
    _window.draw(bgSprite);
}

void Sfml::drawRessources()
{
    auto tex = _textures.find("food");
    if (tex == _textures.end())
        return;

    const sf::Texture &texture = tex->second;
    float originX = texture.getSize().x / 2.0f;
    float originY = texture.getSize().y / 2.0f;
    _sprites["food"].setOrigin(originX, originY);

    float desiredFoodSize = _tileSize * 0.40f; 
    float scaleX = desiredFoodSize / texture.getSize().x;
    float scaleY = desiredFoodSize / texture.getSize().y;
    _sprites["food"].setScale(scaleX, scaleY);

    for (size_t y = 0; y < _world.getMapSize().second; ++y) {
        for (size_t x = 0; x < _world.getMapSize().first; ++x) {
            if (!_world.getMap()[y][x].empty() && _world.getMap()[y][x][0] > 0) {
                float tileX = _offsetX + (x * _tileSize);
                float tileY = _offsetY + (y * _tileSize);
                float centerX = tileX + (_tileSize / 2.0f);
                float centerY = tileY + (_tileSize / 2.0f);
                _sprites["food"].setPosition(centerX, centerY);
                _window.draw(_sprites["food"]);
            }
        }
    }
}

void Sfml::drawEggs()
{
    auto tex = _textures.find("egg");
    if (tex == _textures.end())
        return;

    const sf::Texture &texture = tex->second;
    float originX = texture.getSize().x / 2.0f;
    float originY = texture.getSize().y / 2.0f;
    _sprites["egg"].setOrigin(originX, originY);

    float desiredeggSize = _tileSize * 0.90f; 
    float scaleX = desiredeggSize / texture.getSize().x;
    float scaleY = desiredeggSize / texture.getSize().y;
    _sprites["egg"].setScale(scaleX, scaleY);

    for (const auto &egg : _world.getEggs()) {
        if (egg.size() < 4)
            continue;
        int x = egg[2];
        int y = egg[3];
        float tileX = _offsetX + (x * _tileSize);
        float tileY = _offsetY + (y * _tileSize);
        float centerX = tileX + (_tileSize / 2.0f);
        float centerY = tileY + (_tileSize / 2.0f);

        _sprites["egg"].setPosition(centerX, centerY);
        _window.draw(_sprites["egg"]);
    }
}

void resizeSprite()
{
    
}

void Sfml::drawTrantorians()
{
    auto tex = _textures.find("benoit");
    if (tex == _textures.end() || _world.getTrantorians().empty())
        return;

    const sf::Texture &texture = tex->second;
    _sprites["benoit"].setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);

    float desiredtrantorianSize = _tileSize * 0.95f; 
    _sprites["benoit"].setScale(desiredtrantorianSize / texture.getSize().x, desiredtrantorianSize / texture.getSize().y);

    for (const auto &trantorian : _world.getTrantorians()) {
        _sprites["benoit"].setPosition(trantorian.second.visualPos);
        _window.draw(_sprites["benoit"]);
    }
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
    drawRessources();
    drawEggs();
    drawTrantorians();

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
        //throw une erreur lors de l'ouverture du fichier assets
        return 84;
    }
    return 0;
}

void Sfml::updateAnimations(float deltaTime)
{
    float maxMapWidthPixels = _world.getMapSize().first * _tileSize;
    float maxMapHeightPixels = _world.getMapSize().second * _tileSize;

    for (auto &pair : _world.getTrantorians()) {
        Player_t &player = pair.second;
        sf::Vector2f serverPixelPos = convertToPixels(player.x, player.y);

        if (player.targetPos == sf::Vector2f(0.0f, 0.0f)) {
            player.visualPos = serverPixelPos;
            player.targetPos = serverPixelPos;
            player.isMoving = false;
        }
        if (player.targetPos != serverPixelPos) {
            float distCheckX = std::abs(serverPixelPos.x - player.targetPos.x);
            float distCheckY = std::abs(serverPixelPos.y - player.targetPos.y);

            if (distCheckX > maxMapWidthPixels / 2.0f || distCheckY > maxMapHeightPixels / 2.0f) {
                player.visualPos = serverPixelPos;
                player.targetPos = serverPixelPos;
                player.isMoving = false;
            } else {
                player.targetPos = serverPixelPos;
                player.isMoving = true;
            }
        }
        if (player.isMoving) {
            float dx = player.targetPos.x - player.visualPos.x;
            float dy = player.targetPos.y - player.visualPos.y;

            if (std::abs(dx) < 0.5f && std::abs(dy) < 0.5f) {
                player.visualPos = player.targetPos;
                player.isMoving = false;
            } else {
                player.visualPos.x += dx * 8.0f * deltaTime;
                player.visualPos.y += dy * 8.0f * deltaTime;
            }
        }
    }
}

sf::Vector2f Sfml::convertToPixels(int x, int y) const
{
    float centerX = _offsetX + (x * _tileSize) + (_tileSize / 2.0f);
    float centerY = _offsetY + (y * _tileSize) + (_tileSize / 2.0f);
    return sf::Vector2f(centerX, centerY);
}