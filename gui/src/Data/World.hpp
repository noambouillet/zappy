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

struct Trantorian_t {
    int id;
    int x;
    int y;
    int orientation;
    int level;
    std::string teamName;
    std::string Name;
    std::vector<int> inventory;
};

struct TileData_t {
    std::map<int, Trantorian_t> trantorians;
    std::map<int, int> eggs;
    std::vector<int> ressources;
};

constexpr int NB_RESSOURCE = 7;

class World {
    public:
        World() = default;
        ~World() = default;

        void setMapSize(size_t width, size_t height);
        void setTile(int x, int y, const std::vector<int> &newRessources);
        void setTrantorianLvl(int TrantorianID, int lvl);
        void setTrantorianInventory(int x, int y, int TrantorianID, const std::vector<int> &newRessources);
        void addTeam(const std::string& teamName);
        void setTimeUnit(int timeUnit);
        void updateGameTime(float deltaTime);
        std::string getFormattedGameTime() const;
        void addEgg(int EggNB, int TrantorianID, int x, int y);
        void addTrantorian(Trantorian_t Trantorian);
        void removeEgg(int eggNB);
        void removeTrantorian(int id);
        void moveTrantorian(int id, int newX, int newY, int orientation);
        int getTime() const;
        Trantorian_t &getTrantorian(int id);
        const std::vector<std::vector<TileData_t>>& getMap() const;
        std::pair<size_t, size_t> getMapSize() const;
        TileData_t &getTileData(int x, int y);
        const std::vector<std::string> &getTeams();
        std::map<std::string, std::map<int, int>> getTeamStats() const;
        const std::vector<int> &getTotalRessources();
        void setSelectedTeam(const std::string teamName);
        const std::string &getSelectedTeam() const;
        void setSelectedTrantorianId(int id);
        int getSelectedTrantorianId() const;
        void setHoveredTile(int x, int y);
        std::pair<int, int> getHoveredTile() const;
        void setSelectedTile(int x, int y);
        std::pair<int, int> getSelectedTile() const;
        void setGameOver(const std::string &teamName);
        bool isGameOver() const;
        const std::string &getWinningTeam() const;
        void setDisplayLvl(bool);
        bool getDisplayLvl();

    private:
        std::pair<size_t, size_t> _mapSize{0, 0};
        std::vector<std::vector<TileData_t>> _map;
        std::vector<std::string> _teams;
        std::vector<int> _totalRessources = std::vector<int>(NB_RESSOURCE, 0);
        int _timeUnit = 0;
        float _internalGameTime = 0.0f;
        std::string _selectedTeamName = "";
        int _selectedTrantorianId = -1;
        std::pair<int, int> _hoveredTile = {-1, -1};
        std::pair<int, int> _selectedTile = {-1, -1};
        bool _isGameOver = false;
        std::string _winningTeam = "";
        bool _displayLvl = false;

};

#endif