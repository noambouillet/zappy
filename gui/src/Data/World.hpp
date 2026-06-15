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

struct Player_t {
    int id;
    int x;
    int y;
    int orientation;
    int level;
    std::string teamName;
};

constexpr int NB_RESSOURCE = 7;
struct TileData_t {
    std::map<int, Player_t> players;
    std::map<int, int> eggs;
    std::vector<int> ressources;
};

class World {
    public:
        World() = default;
        ~World() = default;

        void setMapSize(size_t width, size_t height);
        void setTile(int x, int y, const std::vector<int> &ressources);
        void addTeam(const std::string& teamName);
        void setTimeUnit(int timeUnit);

        void addEgg(int EggNB, int playerID, int x, int y);
        void addPlayer(Player_t player);
        void removeEgg(int eggNB);
        void removeTrantorian(int id);
        
        void movePlayer(int id, int newX, int newY, int orientation);
        
        int getTime() const;
        Player_t &getTrantorian(int id);
        const std::vector<std::vector<TileData_t>>& getMap() const;
        std::pair<size_t, size_t> getMapSize() const;
        TileData_t &getTileData(int x, int y);

    private:
        std::pair<size_t, size_t> _mapSize{0, 0};
        std::vector<std::vector<TileData_t>> _map;
        std::vector<std::string> _teams;
        int _timeUnit = 0;
};