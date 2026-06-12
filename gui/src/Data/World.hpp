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
    sf::Vector2f visualPos = {0.0f, 0.0f};
    sf::Vector2f targetPos = {0.0f, 0.0f};
    bool isMoving = false;
    float animationProgress = 0.0f;
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
        sf::Vector2f convertToPixels(int x, int y) const;

        const std::vector<std::vector<std::vector<int>>>& getMap() const { return _map; }
        const std::vector<std::vector<int>>& getEggs() const { return _eggs; }
        std::pair<size_t, size_t> getMapSize() const { return _mapSize; }
        std::map<int, Player_t> &getTrantorians() { return _players; }
        Player_t &getTrantorian(int id) { return _players[id];}

    private:
        std::pair<size_t, size_t> _mapSize{0, 0};
        std::vector<std::vector<std::vector<int>>> _map;
        std::vector<std::vector<int>> _eggs;
        std::vector<std::string> _teams;
        std::map<int, Player_t> _players;
        int _timeUnit = 0;
};