/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** UIRender
*/

#include "UIRender.hpp"
#include "GuiExceptions.hpp"

UIRender::UIRender(World &world, sf::RenderWindow &window, TextureManager &textureManager)
    : _world(world), _window(window), _textureManager(textureManager)
{
    if (!_font.loadFromFile("gui/assets/fonts/magic.ttf"))
        throw GuiException("can't load font");
    _text.setFont(_font);
    _text.setFillColor(sf::Color::White);
}

UIRender::~UIRender() {}

void UIRender::displayUI()
{
    drawText("Time : " + _world.getFormattedGameTime(), 50, 830.0f, 30.0f);
    drawText("Frequency : " + std::to_string(_world.getTime()), 50, 100.0f, 200.0f);
    displayRessources();
    displayTeams();
}

void UIRender::displayRessources()
{
    const std::vector<int> resources = _world.getTotalRessources();
    const std::vector<std::pair<std::string, std::string>> items = {{"donut", "Food : "}, {"linemate", "Linemate : "}, {"linemate", "Deraumere : "}, {"rubis", "Sibur : "}, {"diamond", "Mendiane : "}, {"saphir", "Phiras : "}, {"amethyst", "Thystate : "}};
    const std::vector<float> sizes = {1.5f, 18.0f, 18.0f, 30.0f, 30.0f, 40.0f, 27.0f};
    float yText = 300.0f, yIcon = 340.0f;

    for (size_t i = 0; i < items.size() && i < resources.size(); i++) {
        drawIcon(items[i].first, sizes[i], 50.0f, yIcon);
        drawText(items[i].second + std::to_string(resources[i]), 50, 100.0f, yText);
        yText += 100.0f;
        yIcon += 100.0f;
    }
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
    sprite.setScale(texture.getSize().x * size / 15000.0f, texture.getSize().y * size / 15000.0f);
    sprite.setPosition(x, y);
    _window.draw(sprite);
}

std::vector<Player_t> UIRender::getTeamPlayers(const std::string &teamName) const
{
    std::vector<Player_t> players;
    const auto &map = _world.getMap();

    for (const auto &row : map)
        for (const auto &tile : row)
            for (const auto &pair : tile.players)
                if (pair.second.teamName == teamName)
                    players.push_back(pair.second);
    return players;
}

void UIRender::displayTeams()
{
    const std::vector<std::string> teams = _world.getTeams();
    float x = 1550.0f, y = 160.0f, btnW = 300.0f, btnH = 65.0f;
    sf::Vector2f mousePos = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));

    drawText("Teams :", 50, 1500.0f, 80.0f);
    _hoveredTeam = "";
    int hoveredPlayerId = -1;
    for (const auto &team : teams) {
        sf::Color textColor = sf::Color::White;
        _buttonShape.setOutlineColor(sf::Color(80, 80, 80));
        sf::FloatRect btnBounds(x, y, btnW, btnH);
        if (btnBounds.contains(mousePos)) {
            textColor = sf::Color::Magenta;
            _buttonShape.setOutlineColor(sf::Color::Magenta);
            _hoveredTeam = team;
        }
        if (team == _selectedTeam) {
            textColor = sf::Color::Green;
            _buttonShape.setOutlineColor(sf::Color::Green);
        }
        _text.setFillColor(textColor);
        drawButton(team, 35, x, y, btnW, btnH);
        y += btnH + 15.0f;
        if (team == _selectedTeam) {
            for (const auto &player : getTeamPlayers(_selectedTeam)) {
                float subX = x + 35.0f, subW = btnW - 35.0f, subH = 45.0f;
                sf::FloatRect subBounds(subX, y, subW, subH);
                sf::Color subColor = sf::Color::White;
                _buttonShape.setOutlineColor(sf::Color(100, 100, 100));
                if (subBounds.contains(mousePos)) {
                    subColor = sf::Color::Cyan;
                    _buttonShape.setOutlineColor(sf::Color::Cyan);
                    hoveredPlayerId = player.id;
                }
                _text.setFillColor(subColor);
                drawButton("Player #" + std::to_string(player.id), 25, subX, y, subW, subH);
                y += subH + 10.0f;
            }
        }
    }
    _world.setSelectedTeam(_hoveredTeam);
    _world.setSelectedPlayerId(hoveredPlayerId);
}

void UIRender::handleEvent(const sf::Event &event)
{
    if (event.type != sf::Event::MouseButtonReleased || event.mouseButton.button != sf::Mouse::Left)
        return;
    sf::Vector2f mousePos = _window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
    const std::vector<std::string> teams = _world.getTeams();
    float x = 1550.0f, y = 160.0f, btnW = 300.0f, btnH = 65.0f;
    for (const auto &team : teams) {
        if (sf::FloatRect(x, y, btnW, btnH).contains(mousePos)) {
            _selectedTeam = (_selectedTeam == team) ? "" : team;
            return;
        }
        y += 80.0f;
        if (team == _selectedTeam)
            y += getTeamPlayers(_selectedTeam).size() * 55.0f;
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
    sf::FloatRect bounds = _text.getGlobalBounds();
    _text.setPosition(
        x + (width - bounds.width) / 2.0f - (bounds.left - _text.getPosition().x),
        y + (height - bounds.height) / 2.0f - (bounds.top - _text.getPosition().y)
    );
    _window.draw(_text);
    return _buttonShape.getGlobalBounds();
}