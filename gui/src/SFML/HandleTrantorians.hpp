/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** HandleTrantorians
*/

#pragma once
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include "TextureManager.hpp"

struct PlayerAnim_t {
    int id;
    sf::Vector2f visualPos = {0.0f, 0.0f};
    sf::Vector2f targetPos = {0.0f, 0.0f};
    bool isMoving = false;
    int lastX = -1;
    int lastY = -1;
    float walkTimer = 0.0f;

    std::vector<std::pair<std::string, float>> bubbleQueue;
    float bubbleTimer = 0.0f;

    bool isDying = false;
    int deathFrame = 0;
    float deathTimer = 0.0f;
    bool isDeadAndGone = false;

    bool isIncanting = false;
    int incantFrame = 0;
    float incantTimer = 0.0f;
};

class HandleTrantorians {
    public:
        HandleTrantorians(TextureManager &textureManager, sf::RenderWindow  &window, World &world);
        ~HandleTrantorians();
        void update(float tileSize, float offsetX, float offsetY);
        void drawTrantorians(const TileData_t &tile);
        void setPlayerActionBubble(int id, const std::string &textureKey, float duration);
        void setPlayerIncanting(int id, bool state);
        void triggerPlayerDeath(int id);
        void stopIncantationAt(int x, int y);
        void updateAnimations(float deltaTime);
    private:
        sf::Vector2f convertToPixels(int x, int y) const;
        void updatePlayerAnimation(PlayerAnim_t &anim, const Player_t &player, float deltaTime, float maxWidth, float maxHeight);
        void initPlayerAnim(PlayerAnim_t &anim, const Player_t &player, const sf::Vector2f &pixelPos);
        void updatePlayerPosition(PlayerAnim_t &anim, float deltaTime, float maxWidth, float maxHeight);
        void updatePlayerBubble(PlayerAnim_t &anim, float deltaTime);
        void updatePlayerDeath(PlayerAnim_t &anim, float deltaTime);
        void updatePlayerIncantation(PlayerAnim_t &anim, float deltaTime);
        void drawAnimation(const PlayerAnim_t &anim, float size, int NbFrame, std::string spritesheet, int currentFrame);
        void displayBubble(const PlayerAnim_t &anim);
        void drawBaseIndicator(const sf::Vector2f &pos);

        std::map<int, PlayerAnim_t> _playerAnims;
        TextureManager &_textureManager;
        sf::RenderWindow  &_window;
        World &_world;
        float _tileSize;
        float _offsetX;
        float _offsetY;
};
