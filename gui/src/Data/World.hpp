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

typedef struct Player_s {
    int id;
    int x;
    int y;
    int orientation;
    int level;
    std::string teamName;
} Player_t;

class World {
    public:
        World() = default;
        ~World() = default;

        void setMapSize(size_t width, size_t height);
        void setTile(int x, int y, const std::vector<int> &ressources);
        void addTeam(const std::string& teamName);
        void addEgg(const std::vector<int>& egg);
        void setTimeUnit(int timeUnit);
        void addPlayer(Player_t player);

        const std::vector<std::vector<std::vector<int>>>& getMap() const { return _map; }
        const std::vector<std::vector<int>>& getEggs() const { return _eggs; }
        std::pair<size_t, size_t> getMapSize() const { return _mapSize; }
        std::map<int, Player_t> getTrantorians() const { return _players; }

    private:
        std::pair<size_t, size_t> _mapSize{0, 0};
        std::vector<std::vector<std::vector<int>>> _map;
        std::vector<std::vector<int>> _eggs;
        std::vector<std::string> _teams;
        std::map<int, Player_t> _players;
        int _timeUnit = 0;
};