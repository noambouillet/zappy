/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** World
*/

#pragma once
#include <vector>
#include <string>
#include <map>
#include <SFML/Graphics.hpp>

typedef struct Player_s {
    int id;
    int x;
    int y;
    int orientation;
    int level;
    std::string teamName;
} Player_t;

typedef struct TileData_s {
    std::map<int, Player_t> players;
    std::vector<std::vector<int>> eggs;
    std::vector<int> ressources;
} TileData_t;

class World {
    public:
        World() = default;
        ~World() = default;

        void setMapSize(size_t width, size_t height);
        void setTile(int x, int y, const std::vector<int> &ressources);
        void addTeam(const std::string& teamName);
        void setTimeUnit(int timeUnit);

        void addEgg(const std::vector<int>& egg);
        void addPlayer(Player_t player);
        
        void movePlayer(int id, int newX, int newY, int orientation);
        
        Player_t &getTrantorian(int id);
        const std::vector<std::vector<TileData_t>>& getMap() const { return _map; }
        std::pair<size_t, size_t> getMapSize() const { return _mapSize; }
        TileData_t &getTileData(int x, int y) { return _map[y][x]; }

    private:
        std::pair<size_t, size_t> _mapSize{0, 0};
        std::vector<std::vector<TileData_t>> _map;
        std::vector<std::string> _teams;
        int _timeUnit = 0;
};