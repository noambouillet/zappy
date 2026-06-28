/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RayUI
*/

#include "RayUI.hpp"
#include <iostream>

RayUI::RayUI(World &world, const std::unordered_map<std::string, std::unique_ptr<RayTexture>> &textures): _world(world), _textures(textures)
{
}

Color RayUI::getTeamColor(const std::string& teamName)
{
    static const Color palette[] = {
        RED, BLUE, GREEN, YELLOW, ORANGE, PINK, PURPLE, SKYBLUE, MAGENTA, LIME, GOLD, VIOLET
    };
    unsigned int hash = 0;
    for (char c : teamName)
        hash += static_cast<unsigned int>(c);
    return palette[hash % 12];
}

Color RayUI::getLevelColor(int level)
{
    static const Color levelColors[] = {
        Color{255, 0, 0, 255},
        Color{255, 228, 0, 255},
        Color{165, 255, 0, 255},
        Color{0, 255, 106, 255},
        Color{0, 239, 255, 255},
        Color{0, 90, 255, 255},
        Color{69, 0, 255, 255},
        Color{228, 0, 255, 255}
    };
    if (level >= 1 && level <= 8)
        return levelColors[level - 1];
    return WHITE;
}

void RayUI::drawResourceLine(int x, int y, const std::string &iconKey, const std::string &name, int count)
{
    Color textColor = RAYWHITE;
    if (name == "Linemate")
        textColor = PINK;
    else if (name == "Deraumere")
        textColor = GREEN;
    else if (name == "Sibur")
        textColor = YELLOW;
    else if (name == "Mendiane")
        textColor = RED;
    else if (name == "Phiras")
        textColor = PURPLE;
    else if (name == "Thystame")
        textColor = SKYBLUE;

    if (_textures.find(iconKey) != _textures.end()) {
        Texture2D tex = _textures.at(iconKey)->getTexture();
        float srcSize = static_cast<float>(tex.height);
        float srcX = (static_cast<float>(tex.width) - srcSize) / 2.0f;
        Rectangle source = { srcX, 0.0f, srcSize, srcSize };
        float iconSize = 30.0f;
        Rectangle dest = { static_cast<float>(x), static_cast<float>(y - 5), iconSize, iconSize };

        DrawTexturePro(tex, source, dest, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
        DrawText(TextFormat("%s : %d", name.c_str(), count), x + 40, y, 20, textColor);
    } else {
        DrawText(TextFormat("%s : %d", name.c_str(), count), x, y, 20, textColor);
    }
}

void RayUI::drawGlobalInfo()
{
    if (_world.isPaused()) {
        std::string pauseText = "The simulation is currently in pause, to resume it, press space bar again";
        int pauseTextWidth = MeasureText(pauseText.c_str(), 25);
        DrawText(pauseText.c_str(), (GetScreenWidth() - pauseTextWidth) / 2, 20, 25, RED);
    }
    int width = 300;
    const auto &teams = _world.getTeams();
    auto teamStats = _world.getTeamStats();

    int dynamicHeight = 365 + teams.size() * 20;
    for (const auto &[team, levels] : teamStats)
        dynamicHeight += levels.size() * 18;
    int height = (dynamicHeight > 440) ? dynamicHeight : 440;

    DrawRectangle(10, 10, width, height, ColorAlpha(BLACK, 0.7f));
    DrawRectangleLines(10, 10, width, height, RAYWHITE);
    DrawText("GLOBAL INFO", 20, 20, 20, RAYWHITE);
    DrawText(TextFormat("Time: %s", _world.getFormattedGameTime().c_str()), 20, 50, 20, LIGHTGRAY);
    DrawText(TextFormat("Frequency: %d", _world.getTime()), 20, 70, 20, LIGHTGRAY);
    auto mapSize = _world.getMapSize();
    DrawText(TextFormat("Map Size: %dx%d", mapSize.first, mapSize.second), 20, 90, 20, LIGHTGRAY);
    DrawText("Teams:", 20, 120, 20, RAYWHITE);

    int yOffset = 140;
    for (const auto &team : teams) {
        int totalPlayers = 0;
        for (const auto &[level, count] : teamStats[team])
            totalPlayers += count;
        Color teamColor = getTeamColor(team);
        DrawText(TextFormat("- %s (Total: %d)", team.c_str(), totalPlayers), 30, yOffset, 20, teamColor);
        yOffset += 20;
        for (const auto &[level, count] : teamStats[team]) {
            DrawText(TextFormat("   Lvl %d: %d AI", level, count), 40, yOffset, 15, getLevelColor(level));
            yOffset += 18;
        }
    }

    yOffset += 20;
    DrawText("Global Resources:", 20, yOffset, 20, RAYWHITE);
    yOffset += 30;

    const auto &res = _world.getTotalRessources();
    std::string resNames[] = {"Food", "Linemate", "Deraumere", "Sibur", "Mendiane", "Phiras", "Thystame"};
    std::string iconKeys[] = {"donut", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};

    for (int i = 0; i < 7; i++) {
        if (res.size() > static_cast<size_t>(i)) {
            drawResourceLine(20, yOffset, iconKeys[i], resNames[i], res[i]);
            yOffset += 25;
        }
    }
}

void RayUI::drawTileInfo(int selectedX, int selectedZ, Vector2 mousePos)
{
    if (selectedX < 0 || selectedZ < 0)
        return;

    auto mapSize = _world.getMapSize();
    if (selectedX >= static_cast<int>(mapSize.first) || selectedZ >= static_cast<int>(mapSize.second))
        return;

    auto &tile = _world.getTileData(selectedX, selectedZ);

    int panelWidth = 250;
    int panelHeight = 350;
    int posX = mousePos.x + 20;
    int posY = mousePos.y + 20;

    if (posX + panelWidth > GetScreenWidth())
        posX = GetScreenWidth() - panelWidth - 10;
    if (posY + panelHeight > GetScreenHeight())
        posY = GetScreenHeight() - panelHeight - 10;

    DrawRectangle(posX, posY, panelWidth, panelHeight, ColorAlpha(BLACK, 0.8f));
    DrawRectangleLines(posX, posY, panelWidth, panelHeight, GREEN);
    DrawText(TextFormat("TILE [%d, %d]", selectedX, selectedZ), posX + 10, posY + 10, 20, GREEN);
    int yOffset = posY + 40;
    std::string resNames[] = {"Food", "Linemate", "Deraumere", "Sibur", "Mendiane", "Phiras", "Thystame"};
    std::string iconKeys[] = {"donut", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};

    for (int i = 0; i < 7; i++) {
        int count = (tile.ressources.size() > static_cast<size_t>(i)) ? tile.ressources[i] : 0;
        drawResourceLine(posX + 10, yOffset, iconKeys[i], resNames[i], count);
        yOffset += 25;
    }

    yOffset += 20;
    DrawText("Players on Tile:", posX + 10, yOffset, 20, GREEN);
    yOffset += 25;

    for (const auto &[id, trantorian] : tile.trantorians) {
        DrawText(TextFormat("ID %d (Lvl %d) %s", trantorian.id, trantorian.level, trantorian.teamName.c_str()), posX + 15, yOffset, 15, getLevelColor(trantorian.level));
        yOffset += 20;
        if (yOffset > posY + panelHeight - 20) {
            DrawText("...", posX + 15, yOffset, 15, LIGHTGRAY);
            break;
        }
    }
}

void RayUI::drawTrantorianInfo(int trantorianId, Vector2 mousePos)
{
    try {
        Trantorian_t &trantorian = _world.getTrantorian(trantorianId);
        
        int panelWidth = 300;
        int panelHeight = 350;
        int posX = mousePos.x + 20;
        int posY = mousePos.y + 20;

        if (posX + panelWidth > GetScreenWidth())
            posX = GetScreenWidth() - panelWidth - 10;
        if (posY + panelHeight > GetScreenHeight())
            posY = GetScreenHeight() - panelHeight - 10;

        DrawRectangle(posX, posY, panelWidth, panelHeight, ColorAlpha(BLACK, 0.8f));
        DrawRectangleLines(posX, posY, panelWidth, panelHeight, PURPLE);

        DrawText(TextFormat("WIZARD #%d", trantorian.id), posX + 10, posY + 10, 20, PURPLE);

        int yOffset = posY + 40;
        DrawText(TextFormat("Team: %s", trantorian.teamName.c_str()), posX + 10, yOffset, 15, WHITE);
        yOffset += 20;
        DrawText(TextFormat("Level: %d", trantorian.level), posX + 10, yOffset, 15, getLevelColor(trantorian.level));
        yOffset += 20;
        std::string textOrientation[] = {"North", "East", "South", "West"};
        DrawText(TextFormat("Orientation: %s", textOrientation[trantorian.orientation - 1].c_str()), posX + 10, yOffset, 15, WHITE);

        yOffset += 30;
        DrawText("Inventory:", posX + 10, yOffset, 20, PURPLE);
        yOffset += 25;

        std::string resNames[] = {"Food", "Linemate", "Deraumere", "Sibur", "Mendiane", "Phiras", "Thystame"};
        std::string iconKeys[] = {"donut", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};

        for (int i = 0; i < 7; i++) {
            int count = (trantorian.inventory.size() > static_cast<size_t>(i)) ? trantorian.inventory[i] : 0;
            drawResourceLine(posX + 10, yOffset, iconKeys[i], resNames[i], count);
            yOffset += 25;
        }
    } catch (const std::exception &e) {
        throw;
    }
}

void RayUI::drawGameOverScreen(const std::string &winningTeam)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(BLACK, 0.8f));

    int panelWidth = 700;
    int panelHeight = 400;
    int posX = (GetScreenWidth() - panelWidth) / 2;
    int posY = (GetScreenHeight() - panelHeight) / 2;

    DrawRectangle(posX, posY, panelWidth, panelHeight, ColorAlpha(BLACK, 0.9f));
    DrawRectangleLinesEx(Rectangle{static_cast<float>(posX), static_cast<float>(posY), static_cast<float>(panelWidth), static_cast<float>(panelHeight)}, 3.0f, PURPLE);

    std::string title = "VICTORY !";
    int titleWidth = MeasureText(title.c_str(), 60);
    DrawText(title.c_str(), posX + (panelWidth - titleWidth) / 2, posY + 50, 60, WHITE);

    std::string text = "Team " + winningTeam + " has won the game!";
    int textWidth = MeasureText(text.c_str(), 35);
    DrawText(text.c_str(), posX + (panelWidth - textWidth) / 2, posY + 180, 35, WHITE);

    std::string timeText = "Match Duration: " + _world.getGameOverTime();
    int timeTextWidth = MeasureText(timeText.c_str(), 25);
    DrawText(timeText.c_str(), posX + (panelWidth - timeTextWidth) / 2, posY + 240, 25, GRAY);
}
