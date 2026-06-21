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

struct TrantorianAnim_t {
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

struct BroadcastWave {
    sf::Vector2f centerPos;
    float radius = 0.0f;
};

constexpr float MAX_RADIUS = 100.0f;
constexpr float SPEED = 250.0f;

class HandleTrantorians {
    public:
        HandleTrantorians(TextureManager &textureManager, sf::RenderWindow  &window, World &world);
        ~HandleTrantorians();
        void update(float tileSize, float offsetX, float offsetY);
        void drawTrantorians(const TileData_t &tile);
        void setTrantorianActionBubble(int id, const std::string &textureKey, float duration);
        void setTrantorianIncanting(int id, bool state);
        void triggerTrantorianDeath(int id);
        void stopIncantationAt(int x, int y);
        void updateAnimations(float deltaTime);
        void addBroadcast(int x, int y);
        void handleEvent(const sf::Event &event);
    private:
        sf::Vector2f convertToPixels(int x, int y) const;
        void updateTrantorianAnimation(TrantorianAnim_t &anim, const Trantorian_t &trantorian, float deltaTime, float maxWidth, float maxHeight);
        void initTrantorianAnim(TrantorianAnim_t &anim, const Trantorian_t &trantorian, const sf::Vector2f &pixelPos);
        void updateTrantorianPosition(TrantorianAnim_t &anim, float deltaTime, float maxWidth, float maxHeight);
        void updateTrantorianBubble(TrantorianAnim_t &anim, float deltaTime);
        void updateTrantorianDeath(TrantorianAnim_t &anim, float deltaTime);
        void updateTrantorianIncantation(TrantorianAnim_t &anim, float deltaTime);
        void drawAnimation(const TrantorianAnim_t &anim, float size, int NbFrame, std::string spritesheet, int currentFrame);
        void displayBubble(const TrantorianAnim_t &anim);
        void drawBaseIndicator(const sf::Vector2f &pos);
        void updateAndDrawWaves(sf::RenderWindow &window);

        std::map<int, TrantorianAnim_t> _trantorianAnims;
        TextureManager &_textureManager;
        sf::RenderWindow &_window;
        World &_world;
        std::vector<BroadcastWave> _waves;
        float _tileSize;
        float _offsetX;
        float _offsetY;
        sf::Clock _clock;
        bool _displayBroadcast;
};