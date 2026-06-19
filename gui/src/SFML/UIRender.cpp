/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** UIRender
*/

#include "UIRender.hpp"
#include "GuiExceptions.hpp"


UIRender::UIRender(World &world,sf::RenderWindow &window, TextureManager &textureManager): _world(world), _window(window), _textureManager(textureManager)
{
    if (!_font.loadFromFile("gui/assets/fonts/magic.ttf"))
        throw GuiException("can't load font");
    _text.setFont(_font);
    _text.setFillColor(sf::Color::White);
}

UIRender::~UIRender()
{
}

void UIRender::displayUI()
{
    drawText("Time : " + _world.getFormattedGameTime(), 50, 830.0f, 30.0f);
    drawText("Frequency : " + std::to_string(_world.getTime()), 50, 100.0f, 200.0f);
    displayRessources();
    displayTeams();
}

void UIRender::displayRessources()
{
    const std::vector<int> totalRessources = _world.getTotalRessources();
    drawIcon("donut", 1.5f, 50.0f, 340.0f);
    drawText("Food : " + std::to_string(totalRessources[0]), 50, 100.0f, 300.0f);
    drawIcon("linemate", 18.0f, 50.0f, 440.0f);
    drawText("Linemate : " + std::to_string(totalRessources[1]), 50, 100.0f, 400.0f);
    drawIcon("linemate", 18.0f, 50.0f, 540.0f);
    drawText("Deraumere : " + std::to_string(totalRessources[2]), 50, 100.0f, 500.0f);
    drawIcon("rubis", 30.0f, 50.0f, 630.0f);
    drawText("Sibur : " + std::to_string(totalRessources[3]), 50, 100.0f, 600.0f);
    drawIcon("diamond", 30.0f, 50.0f, 735.0f);
    drawText("Mendiane : " + std::to_string(totalRessources[4]), 50, 100.0f, 700.0f);
    drawIcon("saphir", 40.0f, 50.0f, 830.0f);
    drawText("Phiras : " + std::to_string(totalRessources[5]), 50, 100.0f, 800.0f);
    drawIcon("amethyst", 27.0f, 50.0f, 935.0f);
    drawText("Thystate : " + std::to_string(totalRessources[6]), 50, 100.0f, 900.0f);
}

void UIRender::drawText(std::string text, int size, float x, float y)
{
    _text.setString(text);
    _text.setCharacterSize(size);
    _text.setFillColor(sf::Color::White);
    _text.setPosition(x, y);
    _window.draw(_text);
}

void UIRender::drawIcon(std::string textureKey, float size, float x, float y)
{
    const sf::Texture &texture = _textureManager.getTexture(textureKey);
    sf::Sprite &sprite = _textureManager.getSprite(textureKey);
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    sprite.setScale(texture.getSize().x * size / 15000, texture.getSize().y * size/ 15000);
    sprite.setPosition(x, y);
    _window.draw(sprite);
}

void UIRender::displayTeams()
{
    const std::vector<std::string> teams = _world.getTeams();
    float x = 1550.0f;
    float y = 160.0f;
    float btnWidth = 300.0f;
    float btnHeight = 65.0f;
    sf::Vector2i mousePos = sf::Mouse::getPosition(_window);
    sf::Vector2f mouseWorldPos = _window.mapPixelToCoords(mousePos);

    drawText("Teams :", 50, 1500.0f, 80.0f);
    
    bool isHover = false;
    _hoveredTeam = "";

    for (unsigned long i = 0; i < teams.size(); i++) {
        sf::Color textColor = sf::Color::White;
        _buttonShape.setOutlineColor(sf::Color(80, 80, 80));
        sf::FloatRect btnBounds(x, y, btnWidth, btnHeight);        
        
        if (btnBounds.contains(mouseWorldPos)) {
            textColor = sf::Color::Magenta;
            _buttonShape.setOutlineColor(sf::Color::Magenta);
            _hoveredTeam = teams[i];
            isHover = true;
        }
        if (teams[i] == _selectedTeam) {
            textColor = sf::Color::Green;
            _buttonShape.setOutlineColor(sf::Color::Green);
        }
        _text.setFillColor(textColor); 
        drawButton(teams[i], 35, x, y, btnWidth, btnHeight);
        y += btnHeight + 15.0f; 
        if (teams[i] == _selectedTeam) {
            const auto &map = _world.getMap();
            for (size_t my = 0; my < _world.getMapSize().second; ++my) {
                for (size_t mx = 0; mx < _world.getMapSize().first; ++mx) {
                    for (const auto &pair : map[my][mx].players) {
                        const Player_t &player = pair.second;
                        if (player.teamName == _selectedTeam) {
                            float subBtnX = x + 35.0f;
                            float subBtnWidth = btnWidth - 35.0f;
                            float subBtnHeight = 45.0f;
                            _buttonShape.setOutlineColor(sf::Color(100, 100, 100));
                            _text.setFillColor(sf::Color::White);
                            drawButton("Player #" + std::to_string(player.id), 25, subBtnX, y, subBtnWidth, subBtnHeight);
                            y += subBtnHeight + 10.0f; 
                        }
                    }
                }
            }
        }
    }
    if (isHover)
        _world.setSelectedTeam(_hoveredTeam);
    else
        _world.setSelectedTeam("");
}

void UIRender::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouseWorldPos = _window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        const std::vector<std::string> teams = _world.getTeams();
        float x = 1550.0f;
        float y = 160.0f;

        for (unsigned long i = 0; i < teams.size(); i++) {
            sf::FloatRect btnBounds(x, y, 300.0f, 65.0f);            
            if (btnBounds.contains(mouseWorldPos)) {
                if (_selectedTeam == teams[i])
                    _selectedTeam = "";
                else
                    _selectedTeam = teams[i];
                return;
            }
            y += 80.0f;
            if (teams[i] == _selectedTeam) {
                const auto &map = _world.getMap();
                for (size_t my = 0; my < _world.getMapSize().second; ++my) {
                    for (size_t mx = 0; mx < _world.getMapSize().first; ++mx) {
                        for (const auto &pair : map[my][mx].players) {
                            if (pair.second.teamName == _selectedTeam) {
                                y += 55.0f;
                            }
                        }
                    }
                }
            }
        }
    }
}

sf::FloatRect UIRender::drawButton(std::string text, int fontSize, float x, float y, float width, float height)
{
    _buttonShape.setPosition(x, y);
    _buttonShape.setSize(sf::Vector2f(width, height));
    _buttonShape.setFillColor(sf::Color::Black);
    _buttonShape.setOutlineThickness(2.0f);
    _window.draw(_buttonShape);
    _text.setString(text);
    _text.setCharacterSize(fontSize);
    sf::FloatRect textBounds = _text.getGlobalBounds();
    float textX = x + (width - textBounds.width) / 2.0f - (textBounds.left - _text.getPosition().x);
    float textY = y + (height - textBounds.height) / 2.0f - (textBounds.top - _text.getPosition().y);
    _text.setPosition(textX, textY);
    _window.draw(_text);
    return _buttonShape.getGlobalBounds();
}
