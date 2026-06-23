/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RaylibGui
*/

#ifndef RAYLIBGUI_HPP_
#define RAYLIBGUI_HPP_

#include "Interfaces/IGui.hpp"
#include "Encapsulations/RayWindow.hpp"
#include "Encapsulations/RayCamera.hpp"
#include "Encapsulations/RayTexture.hpp"
#include "Encapsulations/RayModel.hpp"
#include "World.hpp"
#include "RayUI.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

struct RayPlayerAnim_t {
    int id;
    bool isDying = false;
    float deathTimer = 0.0f;
    bool isDeadAndGone = false;
    bool isIncanting = false;
    float incantTimer = 0.0f;
    bool isBroadcasting = false;
    float broadcastTimer = 0.0f;
    std::vector<std::pair<std::string, float>> bubbleQueue;
    float bubbleTimer = 0.0f;
};

class RaylibGui : public IGui {
    public:
        RaylibGui(World &world);
        ~RaylibGui() override = default;

        bool isOpen() const override;
        void handleEvent() override;
        void displayWindow() override;
        
        void setTrantorianActionBubble(int id, const std::string &textureKey, float duration) override;
        void triggerTrantorianDeath(int id) override;
        void setTrantorianIncanting(int id, bool state) override;
        void stopIncantationAt(int x, int y) override;
        void addBroadcast(int id) override;

    private:
        World &_world;
        RayWindow _window;
        RayCamera _camera;
        std::unique_ptr<RayTexture> _backgroundTexture;
        std::unordered_map<std::string, std::unique_ptr<RayTexture>> _textures;
        std::unordered_map<std::string, std::unique_ptr<RayModel>> _models;
        std::unordered_map<std::string, float> _modelScales;
        std::unordered_map<int, RayPlayerAnim_t> _playerAnims;
        std::unique_ptr<RayUI> _ui;
        int _selectedTileX = -1;
        int _selectedTileZ = -1;

        void drawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color);
        void drawTileContent(int x, int z);
        void updateAnimations(float deltaTime);
        void drawBubble(const RayPlayerAnim_t &anim, Vector3 position);
};

#endif /* !RAYLIBGUI_HPP_ */
