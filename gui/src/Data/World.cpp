/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** World
*/

#include "World.hpp"
#include "GuiExceptions.hpp"
#include <stdexcept>

void World::setMapSize(size_t width, size_t height)
{
    _mapSize = {width, height};
    _map.resize(height);
    for (size_t y = 0; y < height; y++) {
        _map[y].resize(width);
        for (size_t x = 0; x < width; x++) {
            _map[y][x].ressources.resize(NB_RESSOURCE, 0);
            _map[y][x].trantorians.clear();
            _map[y][x].eggs.clear();
        }
    }
}

void World::setTile(int x, int y, const std::vector<int> &newResources)
{
    TileData_t &tile = _map[y][x];

    if (!tile.ressources.empty())
        for (size_t i = 0; i < 7; i++)
            _totalRessources[i] -= tile.ressources[i];
    tile.ressources = newResources;
    for (size_t i = 0; i < 7; i++)
        _totalRessources[i] += tile.ressources[i];
}

void World::addEgg(int eggNB, int trantorianID, int x, int y)
{
    _map[y][x].eggs[eggNB] = trantorianID;
}

void World::addTrantorian(Trantorian_t trantorian)
{ 
    _map[trantorian.y][trantorian.x].trantorians[trantorian.id] = trantorian;
}

void World::removeEgg(int eggNB)
{
    for (size_t y = 0; y < _mapSize.second; y++) {
        for (size_t x = 0; x < _mapSize.first; x++) {
            auto it = _map[y][x].eggs.find(eggNB);
            if (it != _map[y][x].eggs.end()) {
                _map[y][x].eggs.erase(it);
                return;
            }
        }
    }
    throw GuiException("Egg ID not found in TileData");
}

void World::removeTrantorian(int id)
{
    for (size_t y = 0; y < _mapSize.second; y++) {
        for (size_t x = 0; x < _mapSize.first; x++) {
            auto it = _map[y][x].trantorians.find(id);
            if (it != _map[y][x].trantorians.end()) {
                _map[y][x].trantorians.erase(it);
                return;
            }
        }
    }
    throw GuiException("Trantorian ID not found in TileData");
}


Trantorian_t &World::getTrantorian(int id)
{
    for (size_t y = 0; y < _mapSize.second; y++) {
        for (size_t x = 0; x < _mapSize.first; x++) {
            auto it = _map[y][x].trantorians.find(id);
            if (it != _map[y][x].trantorians.end()) {
                return it->second;
            }
        }
    }
    throw GuiException("Trantorian ID not found in TileData");
}

void World::setTrantorianLvl(int trantorianID, int lvl)
{
    Trantorian_t &trantorian = getTrantorian(trantorianID);
    trantorian.level = lvl;
}


void World::setTrantorianInventory(int x, int y, int trantorianID, const std::vector<int> &newRessources)
{
    auto trantorian = _map[y][x].trantorians.find(trantorianID);
    
    if (trantorian != _map[y][x].trantorians.end()) {
        trantorian->second.inventory = newRessources;
    }
}


void World::moveTrantorian(int id, int newX, int newY, int orientation)
{
    int oldX = -1;
    int oldY = -1;
    bool found = false;

    for (size_t y = 0; y < _mapSize.second; y++) {
        for (size_t x = 0; x < _mapSize.first; x++) {
            if (_map[y][x].trantorians.find(id) != _map[y][x].trantorians.end()) {
                oldX = x;
                oldY = y;
                found = true;
                break;
            }
        }
        if (found)
            break;
    }
    if (!found)
        return;
    Trantorian_t trantorian = _map[oldY][oldX].trantorians[id];
    trantorian.x = newX;
    trantorian.y = newY;
    trantorian.orientation = orientation;
    if (oldX != newX || oldY != newY) {
        _map[oldY][oldX].trantorians.erase(id);
        _map[newY][newX].trantorians[id] = trantorian;
    } else {
        _map[oldY][oldX].trantorians[id] = trantorian;
    }
}

void World::setTimeUnit(int timeUnit)
{ 
    _timeUnit = timeUnit;
}

void World::addTeam(const std::string& teamName)
{
    _teams.push_back(teamName);
}

const std::vector<std::vector<TileData_t>>& World::getMap() const
{
    return _map;
}

std::pair<size_t, size_t> World::getMapSize() const
{
    return _mapSize;
}

TileData_t &World::getTileData(int x, int y)
{
    return _map[y][x];
}

int World::getTime() const
{
    return _timeUnit;
}

const std::vector<int> &World::getTotalRessources()
{
    return _totalRessources;
}

const std::vector<std::string> &World::getTeams()
{
    return _teams;
}

void World::updateGameTime(float deltaTime)
{
    if (getTime() > 0)
        _internalGameTime += deltaTime * getTime();
}

std::string World::getFormattedGameTime() const
{
    long long totalSeconds = static_cast<long long>(_internalGameTime);
    long long hours = totalSeconds / 3600;
    long long minutes = (totalSeconds % 3600) / 60;
    long long seconds = totalSeconds % 60;
    char buffer[32];

    std::snprintf(buffer, sizeof(buffer), "%02lld:%02lld:%02lld", hours, minutes, seconds);
    return std::string(buffer);
}

void World::setSelectedTeam(const std::string teamName)
{
    _selectedTeamName = teamName;
}

const std::string &World::getSelectedTeam() const
{
    return _selectedTeamName;
}

void World::setSelectedTrantorianId(int id)
{
    _selectedTrantorianId = id;
}

int World::getSelectedTrantorianId() const
{
    return _selectedTrantorianId;
}

void World::setHoveredTile(int x, int y)
{ 
    _hoveredTile = {x, y};
}

std::pair<int, int> World::getHoveredTile() const
{ 
    return _hoveredTile;
}

void World::setSelectedTile(int x, int y)
{ 
    _selectedTile = {x, y};
}

std::pair<int, int> World::getSelectedTile() const
{ 
    return _selectedTile;
}

void World::setGameOver(const std::string &teamName)
{
    _isGameOver = true; _winningTeam = teamName;
}

bool World::isGameOver() const
{
    return _isGameOver;
}

const std::string &World::getWinningTeam() const
{
    return _winningTeam;
}
