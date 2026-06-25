/*
** EPITECH PROJECT, 2026
** PlayerData
** File description:
** PlayerData class
*/

#ifndef PLAYERDATA_HPP_
#define PLAYERDATA_HPP_

#include <string>
#include <queue>

namespace ZappyServer {

struct QueuedCommand {
    std::string line;
    unsigned int remainingTicks;
    bool started;
};

class PlayerData {
    private:
        unsigned int _x;
        unsigned int _y;
        unsigned int _direction;
        unsigned int _level;
        unsigned int _inventory[7];
        unsigned int _foodTicks;
        bool _incantating;
        std::queue<QueuedCommand> _commandQueue;
    public:
        PlayerData();

        void setX(unsigned int x);
        void setY(unsigned int y);
        void setDirection(unsigned int direction);
        void setLevel(unsigned int level);
        unsigned int getX() const;
        unsigned int getY() const;
        unsigned int getDirection() const;
        unsigned int getLevel() const;
        unsigned int getInventory(unsigned int index) const;
        void setInventory(unsigned int index, unsigned int value);
        unsigned int getFoodTicks() const;
        void setFoodTicks(unsigned int value);
        bool isIncantating() const;
        void setIncantating(bool value);
        std::queue<QueuedCommand> &getCommandQueue();
        const std::queue<QueuedCommand> &getCommandQueue() const;
        void queueCommand(const std::string &cmd, unsigned int ticks);
        void popCommand();
};

}

#endif /* !PLAYERDATA_HPP_ */
