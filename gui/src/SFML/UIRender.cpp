/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** UIRender
*/

#include "UIRender.hpp"
#include "GuiExceptions.hpp"

UIRender::UIRender(World &world, sf::RenderWindow &window, TextureManager &textureManager): _world(world), _window(window), _textureManager(textureManager)
{
    if (!_font.loadFromFile("gui/assets/fonts/magic.ttf"))
        throw GuiException("can't load font");
    _text.setFont(_font);
    _text.setFillColor(sf::Color::White);
}

UIRender::~UIRender() {}

void UIRender::displayUI()
{
    if (_world.isGameOver()) {
        displayGameOver();
        return;
    }
    drawPanel(10.0f, 200.0f, 360.0f, 800.0f);
    drawPanel(810.0f, 35.0f, 330.0f, 60.0f);
    drawText("Time : " + _world.getFormattedGameTime(), 50, 830.0f, 30.0f);
    drawText("Frequency : " + std::to_string(_world.getTime()), 50, 60.0f, 200.0f);
    displayRessources();
    displayTeams();
    if (_world.getDisplayLvl())
        displayLevelLegend();
    if (_activeDataTrantorianId != -1)
        displayTrantorianData(_activeDataTrantorianId);
    else if (_world.getSelectedTile().first != -1)
        displayTileData(_world.getSelectedTile().first, _world.getSelectedTile().second);
}

void UIRender::displayRessources()
{
    const std::vector<int> resources = _world.getTotalRessources();
    const std::vector<std::pair<std::string, std::string>> items = {{"donut", "Food : "}, {"linemate", "Linemate : "}, {"emeraude", "Deraumere : "}, {"rubis", "Sibur : "}, {"diamond", "Mendiane : "}, {"saphir", "Phiras : "}, {"amethyst", "Thystate : "}};
    const std::vector<float> sizes = {1.5f, 18.0f, 36.0f, 30.0f, 30.0f, 40.0f, 27.0f};
    float y = 300.0f;

    for (size_t i = 0; i < items.size() && i < resources.size(); i++) {
        drawIcon(items[i].first, sizes[i], 50.0f, y + 40.0f);
        drawText(items[i].second + std::to_string(resources[i]), 50, 100.0f, y);
        y += 100.0f;
    }
}

void UIRender::displayTileData(int x, int y)
{
    try {
        TileData_t &tile = _world.getTileData(x, y);
        float boxX = 20.0f, boxY = 100.0f, boxW = 450.0f, boxH = 800.0f;
        float closeW = 40.0f, closeH = 40.0f;
        float closeX = boxX + boxW - closeW - 15.0f;
        float closeY = boxY + 15.0f;
        
        drawPanel(boxX, boxY, boxW, boxH);
        sf::Vector2f mousePos = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));
        if (sf::FloatRect(closeX, closeY, closeW, closeH).contains(mousePos))
            _buttonShape.setOutlineColor(sf::Color(151, 26, 251));
        else
            _buttonShape.setOutlineColor(sf::Color(150, 150, 150));
        drawButton("X", 25, closeX, closeY, closeW, closeH);
        drawText("Tile X: " + std::to_string(x) + " Y: " + std::to_string(y), 35, boxX + 30.0f, boxY + 20.0f);
        drawText("Resources details :", 40, boxX + 30.0f, boxY + 90.0f);
        const std::vector<std::pair<std::string, std::string>> items = {{"donut", "Food : "}, {"linemate", "Linemate : "}, {"emeraude", "Deraumere : "}, {"rubis", "Sibur : "}, {"diamond", "Mendiane : "}, {"saphir", "Phiras : "}, {"amethyst", "Thystate : "}};
        const std::vector<float> sizes = {0.75f, 9.0f, 18.0f, 15.0f, 15.0f, 20.0f, 13.0f};
        float deltaY = 180.0f + boxY;
        for (size_t i = 0; i < items.size() && i < tile.ressources.size(); i++) {
            drawIcon(items[i].first, sizes[i], 50.0f + boxX, deltaY + 25.0f);
            drawText(items[i].second + std::to_string(tile.ressources[i]), 30, 100.0f + boxX, deltaY);
            deltaY += 50.0f;
        }
        drawText("Trantorians on tile: " + std::to_string(tile.trantorians.size()), 30, boxX + 30.0f, deltaY + 20.0f);
        drawText("Eggs on tile: " + std::to_string(tile.eggs.size()), 30, boxX + 30.0f, deltaY + 60.0f);
    } catch (const std::exception &e) {
        _world.setSelectedTile(-1, -1);
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

std::vector<Trantorian_t> UIRender::getTeamTrantorians(const std::string &teamName) const
{
    std::vector<Trantorian_t> trantorians;
    const auto &map = _world.getMap();

    for (const auto &row : map)
        for (const auto &tile : row)
            for (const auto &pair : tile.trantorians)
                if (pair.second.teamName == teamName)
                    trantorians.push_back(pair.second);
    return trantorians;
}

void UIRender::displayTeams()
{
    const std::vector<std::string> teams = _world.getTeams();
    float x = 1550.0f;
    float y = 160.0f;
    float btnW = 300.0f;
    float btnH = 65.0f;
    sf::Vector2f mousePos = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));

    drawText("Teams :", 50, 1500.0f, 80.0f);
    _hoveredTeam = "";
    int hoveredTrantorianId = -1;
    for (const auto &team : teams) {
        sf::Color textColor = sf::Color::White;
        _buttonShape.setOutlineColor(sf::Color(80, 80, 80));
        sf::FloatRect btnBounds(x, y, btnW, btnH);
        if (btnBounds.contains(mousePos)) {
            textColor = sf::Color(98, 28, 145);
            _buttonShape.setOutlineColor(sf::Color(98, 28, 145));
            _hoveredTeam = team;
        }
        if (team == _selectedTeam) {
            textColor = sf::Color::Magenta;
            _buttonShape.setOutlineColor(sf::Color::Magenta);
        }
        _text.setFillColor(textColor);
        drawButton(team, 35, x, y, btnW, btnH);
        y += btnH + 15.0f;
        if (team == _selectedTeam) {
            for (const auto &trantorian : getTeamTrantorians(_selectedTeam)) {
                float subX = x + 35.0f, subW = btnW - 35.0f, subH = 45.0f;
                sf::FloatRect subBounds(subX, y, subW, subH);
                sf::Color subColor = sf::Color::White;
                _buttonShape.setOutlineColor(sf::Color(100, 100, 100));
                if (subBounds.contains(mousePos)) {
                    _buttonShape.setOutlineColor(sf::Color(151, 26, 251));
                    hoveredTrantorianId = trantorian.id;
                }
                _text.setFillColor(subColor);
                drawButton((trantorian.Name.find(' ') != std::string::npos ? trantorian.Name.substr(0, trantorian.Name.find(' ')) : trantorian.Name) + " Lvl:" + std::to_string(trantorian.level), 35, subX, y, subW, subH);
                y += subH + 10.0f;
            }
        }
    }
    _world.setSelectedTeam(_hoveredTeam);
    _world.setSelectedTrantorianId(hoveredTrantorianId);
}

void UIRender::handleEvent(const sf::Event &event)
{
    if (event.type != sf::Event::MouseButtonReleased || event.mouseButton.button != sf::Mouse::Left)
        return;
    sf::Vector2f mousePos = _window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

    if (_activeDataTrantorianId != -1) {
        float boxX = 20.0f, boxY = 100.0f, boxW = 450.0f;
        float closeW = 40.0f, closeH = 40.0f;
        if (sf::FloatRect(boxX + boxW - closeW - 15.0f, boxY + 15.0f, closeW, closeH).contains(mousePos)) {
            _activeDataTrantorianId = -1;
            return;
        }
    }
    if (_world.getSelectedTile().first != -1) {
        float boxX = 20.0f, boxY = 100.0f, boxW = 450.0f;
        float closeW = 40.0f, closeH = 40.0f;
        float closeX = boxX + boxW - closeW - 15.0f;
        float closeY = boxY + 15.0f;
        if (sf::FloatRect(closeX, closeY, closeW, closeH).contains(mousePos)) {
            _world.setSelectedTile(-1, -1);
            return;
        }
    }
    const std::vector<std::string> teams = _world.getTeams();
    float y = 160.0f;
    for (const auto &team : teams) {
        if (sf::FloatRect(1550.0f, y, 300.0f, 65.0f).contains(mousePos)) {
            _selectedTeam = (_selectedTeam == team) ? "" : team;
            _activeDataTrantorianId = -1;
            return;
        }
        y += 80.0f;
        if (team == _selectedTeam) {
            for (const auto &trantorian : getTeamTrantorians(_selectedTeam)) {
                if (sf::FloatRect(1585.0f, y, 265.0f, 45.0f).contains(mousePos)) {
                    _activeDataTrantorianId = trantorian.id;
                    return;
                }
                y += 45.0f + 10.0f;
            }
        }
    }
    _activeDataTrantorianId = -1;
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
    _text.setPosition(x + (width - bounds.width) / 2.0f - (bounds.left - _text.getPosition().x), y + (height - bounds.height) / 2.0f - (bounds.top - _text.getPosition().y));
    _window.draw(_text);
    return _buttonShape.getGlobalBounds();
}

void UIRender::displayTrantorianData(int trantorianId)
{
    try {
        Trantorian_t &trantorian = _world.getTrantorian(trantorianId);
        float boxX = 20.0f;
        float boxY = 100.0f;
        float closeW = 40.0f;
        float closeH = 40.0f;
        float closeX = boxX + 450.0f - closeW - 15.0f;
        float closeY = boxY + 15.0f;

        drawPanel(boxX, boxY, 450.0f, 800.0f);
        sf::Vector2f mousePos = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));
        if (sf::FloatRect(closeX, closeY, closeW, closeH).contains(mousePos))
            _buttonShape.setOutlineColor(sf::Color(151, 26, 251));
        else
            _buttonShape.setOutlineColor(sf::Color(150, 150, 150));
        drawButton("X", 25, closeX, closeY, closeW, closeH);
        std::string textOrientation[4] = {"North", "East", "South", "West"};
        std::string orientation = textOrientation[trantorian.orientation - 1];
        drawText(trantorian.Name + "  #" + std::to_string(trantorian.id), 35, boxX + 80.0f, boxY + 20.0f);
        drawIcon("trantorian", 2.0f, boxX + 40.0f, boxY + 40.0f);
        drawText("Team: " + trantorian.teamName, 40, boxX + 30.0f, boxY + 90.0f);
        drawText("Level: " + std::to_string(trantorian.level), 40, boxX + 30.0f, boxY + 140.0f);
        drawText("Position: X=" + std::to_string(trantorian.x) + " Y=" + std::to_string(trantorian.y), 40, boxX + 30.0f, boxY + 190.0f);
        drawText("Orientation: " + orientation, 40, boxX + 30.0f, boxY + 240.0f);
        drawText("Inventory: ", 40, boxX + 30.0f, boxY + 310.0f);
        const std::vector<std::pair<std::string, std::string>> items = {{"donut", "Food : "}, {"linemate", "Linemate : "}, {"emeraude", "Deraumere : "}, {"rubis", "Sibur : "}, {"diamond", "Mendiane : "}, {"saphir", "Phiras : "}, {"amethyst", "Thystate : "}};
        const std::vector<float> sizes = {0.75f, 9.0f, 18.0f, 15.0f, 15.0f, 20.0f, 13.0f};
        float yText = 400.0f + boxY;
        float yIcon = 425.0f + boxY;
        for (size_t i = 0; i < items.size() && i < trantorian.inventory.size(); i++) {
            drawIcon(items[i].first, sizes[i], 50.0f + boxX, yIcon);
            drawText(items[i].second + std::to_string(trantorian.inventory[i]), 30, 100.0f + boxX, yText);
            yText += 50.0f;
            yIcon += 50.0f;
        }
    } catch (const std::exception &e) {
        _activeDataTrantorianId = -1;
    }
}

void UIRender::drawPanel(float x, float y, float width, float height)
{
    _buttonShape.setPosition(x, y);
    _buttonShape.setSize(sf::Vector2f(width, height));
    _buttonShape.setFillColor(sf::Color(0, 0, 0, 230));
    _buttonShape.setOutlineColor(sf::Color(151, 26, 251));
    _buttonShape.setOutlineThickness(3.0f);
    _window.draw(_buttonShape);
}

void UIRender::displayGameOver()
{
    sf::RectangleShape overlay(sf::Vector2f(1920.0f, 1080.0f));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    _window.draw(overlay);
    drawPanel(610.0f, 340.0f, 700.0f, 400.0f);
    drawText("VICTORY !", 60, 610.0f + (700.0f / 2.0f) - 140.0f, 340.0f + 50.0f);
    drawText("Team \"" + _world.getWinningTeam() + "\" has won the game!", 35, 610.0f + 60.0f, 340.0f + 180.0f);
}

void UIRender::displayLevelLegend()
{
    static const sf::Color levelColors[] = {sf::Color(255, 0, 0), sf::Color(255, 228, 0), sf::Color(165, 255, 0), sf::Color(0, 255, 106), sf::Color(0, 239, 255), sf::Color(0, 90, 255), sf::Color(69, 0, 255), sf::Color(228, 0, 255)};
    float squareSize = 35.0f;
    float spacing = 15.0f;
    int totalLevels = 8;
    float totalWidth = (squareSize * totalLevels) + (spacing * (totalLevels - 1));
    float startX = (1920.0f - totalWidth) / 2.0f;
    float startY = 1020.0f;

    drawText("LEVELS : ", 25, startX - 100.0f, startY);
    sf::RectangleShape rect(sf::Vector2f(squareSize, squareSize));
    rect.setOutlineColor(sf::Color(40, 40, 40));
    rect.setOutlineThickness(1.5f);
    for (int i = 0; i < totalLevels; i++) {
        float currentX = startX + i * (squareSize + spacing);
        rect.setFillColor(levelColors[i]);
        rect.setPosition(currentX, startY);
        _window.draw(rect);
        drawText(std::to_string(i + 1), 30, currentX + 12.0f, startY - 1.0f);
    }
}