/*
** EPITECH PROJECT, 2026
** Client
** File description:
** Client class
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <string>
#include <queue>

namespace ZappyServer {

struct QueuedCommand {
    std::string line;
    unsigned int remainingTicks;
};

struct ClientState {
    static const int WAITING_TEAM = 0;
    static const int AI = 1;
    static const int GUI = 2;
};

class Client {
    private:
        int _fd;
        int _state;
        std::string _teamName;
        std::string _readBuffer;
        unsigned int _x;
        unsigned int _y;
        unsigned int _direction;
        unsigned int _level;
        unsigned int _inventory[7];
        unsigned int _foodTicks;
        std::queue<QueuedCommand> _commandQueue;
    public:
        explicit Client(int fd);

        int getFd() const;
        int getState() const;
        const std::string &getTeamName() const;
        std::string &getReadBuffer();
        void setState(int newState);
        void setTeamName(const std::string &newTeamName);
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
        std::queue<QueuedCommand> &getCommandQueue();
        void queueCommand(const std::string &cmd, unsigned int ticks);
        void invalidate();
        bool isDead() const;
};

}

#endif /* !CLIENT_HPP_ */
