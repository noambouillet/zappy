/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** World
*/

#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <vector>
#include <string>
#include <map>

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
        void setTile(int x, int y, const std::vector<int> &newRessources);
        void addTeam(const std::string& teamName);
        void setTimeUnit(int timeUnit);
        void updateGameTime(float deltaTime);
        std::string getFormattedGameTime() const;
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
        const std::vector<std::string> &getTeams();
        const std::vector<int> &getTotalRessources();
        void setSelectedTeam(const std::string teamName);
        const std::string &getSelectedTeam() const;
        void setSelectedPlayerId(int id);
        int getSelectedPlayerId() const;

    private:
        std::pair<size_t, size_t> _mapSize{0, 0};
        std::vector<std::vector<TileData_t>> _map;
        std::vector<std::string> _teams;
        std::vector<int> _totalRessources = std::vector<int>(7, 0);
        int _timeUnit = 0;
        float _internalGameTime = 0.0f;
        std::string _selectedTeamName = "";
        int _selectedPlayerId = -1;
};

#endif /* !WORLD_HPP_ */
