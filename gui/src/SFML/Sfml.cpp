/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** Sfml
*/

#include "Sfml.hpp"
#include "Logger.hpp"
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

void Sfml::drawRessources(int x, int y, const TileData_t &tile)
{
    float centerX = _offsetX + (x * _tileSize) + (_tileSize / 2.0f);
    float centerY = _offsetY + (y * _tileSize) + (_tileSize / 2.0f);
    float desiredFoodSize = _tileSize * 0.40f; 

    auto tex = _textures.find("food");
    if (tex != _textures.end() && !tile.ressources.empty() && tile.ressources[0] > 0) {
        const sf::Texture &texture = tex->second;
        _sprites["food"].setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        _sprites["food"].setScale(desiredFoodSize / texture.getSize().x, desiredFoodSize / texture.getSize().y);
        _sprites["food"].setPosition(centerX, centerY);
        _window.draw(_sprites["food"]);
    }
}

void Sfml::drawEggs(int x, int y, const TileData_t &tile)
{
    float centerX = _offsetX + (x * _tileSize) + (_tileSize / 2.0f);
    float centerY = _offsetY + (y * _tileSize) + (_tileSize / 2.0f);
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    float desiredEggSize = _tileSize * 0.90f;

    auto tex = _textures.find("egg");
    if (tex != _textures.end() && !tile.eggs.empty()) {
        const sf::Texture &texture = tex->second;
        _sprites["egg"].setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        _sprites["egg"].setScale(desiredEggSize / texture.getSize().x, desiredEggSize / texture.getSize().y);
        for (size_t i = 0; i < tile.eggs.size(); ++i) {
            if (tile.eggs.size() > 1) {
                offsetX = static_cast<float>((i % 3) * (_tileSize * 0.25f) - (_tileSize * 0.25f));
                offsetY = static_cast<float>((i / 3) * (_tileSize * 0.25f) - (_tileSize * 0.25f));
            }
            _sprites["egg"].setPosition(centerX + offsetX, centerY + offsetY);
            _window.draw(_sprites["egg"]);
        }
    }
}

void Sfml::drawTrantorians(const TileData_t &tile)
{
    float desiredTrantorianSize = _tileSize * 0.95f; 

    auto tex = _textures.find("benoit");
    if (tex != _textures.end() && !tile.players.empty()) {
        const sf::Texture &texture = tex->second;
        _sprites["benoit"].setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        _sprites["benoit"].setScale(desiredTrantorianSize / texture.getSize().x, desiredTrantorianSize / texture.getSize().y);
        for (const auto &pair : tile.players) {
            const Player_t &player = pair.second;
            auto animIt = _playerAnims.find(player.id);
            if (animIt != _playerAnims.end()) {
                _sprites["benoit"].setPosition(animIt->second.visualPos);
                _window.draw(_sprites["benoit"]);
            }
        }
    }
}

void Sfml::drawTileElements(int x, int y, const TileData_t &tile)
{
    drawRessources(x, y, tile);
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
                logger.error("fail to load " + entry.path().string());
                continue;
            }
            _textures[entry.path().stem().string()] = std::move(texture);
            sprite.setTexture(_textures[entry.path().stem().string()]);
            _sprites[entry.path().stem().string()] = std::move(sprite);
            logger.info("texture loaded : " + entry.path().stem().string());
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

    for (size_t y = 0; y < _world.getMapSize().second; ++y) {
        for (size_t x = 0; x < _world.getMapSize().first; ++x) {
            TileData_t &tile = _world.getTileData(x, y);

            for (const auto &pair : tile.players) {
                const Player_t &player = pair.second;
                PlayerAnim_t &anim = _playerAnims[player.id];
                sf::Vector2f currentTilePixelPos = convertToPixels(player.x, player.y);

                if (anim.visualPos == sf::Vector2f(0.0f, 0.0f) && currentTilePixelPos != sf::Vector2f(0.0f, 0.0f)) { // si le player vient d'être créer
                    anim.id = player.id;
                    anim.visualPos = currentTilePixelPos;
                    anim.targetPos = currentTilePixelPos;
                    anim.lastX = player.x;
                    anim.lastY = player.y;
                    anim.isMoving = false;
                }
                if (player.x != anim.lastX || player.y != anim.lastY) { //si le player s'est déplacé
                    anim.targetPos = currentTilePixelPos;
                    anim.isMoving = true;
                    anim.lastX = player.x;
                    anim.lastY = player.y;
                }
                if (anim.isMoving) { // si le player atteint l'une des limites
                    if (std::abs(anim.targetPos.x - anim.visualPos.x) > maxMapWidthPixels / 2.0f || std::abs(anim.targetPos.y - anim.visualPos.y) > maxMapHeightPixels / 2.0f) {
                        anim.visualPos = anim.targetPos;
                        anim.isMoving = false;
                    }
                }
                if (anim.isMoving) { //fonction de lerp pour la fluidité des mouvement
                    float dx = anim.targetPos.x - anim.visualPos.x;
                    float dy = anim.targetPos.y - anim.visualPos.y;

                    if (std::abs(dx) < 1.0f && std::abs(dy) < 1.0f) {
                        anim.visualPos = anim.targetPos;
                        anim.isMoving = false;
                    } else {
                        anim.visualPos.x += dx * 12.0f * deltaTime;
                        anim.visualPos.y += dy * 12.0f * deltaTime;
                    }
                }
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