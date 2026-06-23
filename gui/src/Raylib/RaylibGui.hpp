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
#include <memory>
#include <unordered_map>
#include <string>

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
        void addBroadcast(int x, int y) override;

    private:
        World &_world;
        RayWindow _window;
        RayCamera _camera;
        std::unique_ptr<RayTexture> _backgroundTexture;
        std::unordered_map<std::string, std::unique_ptr<RayTexture>> _textures;
        std::unordered_map<std::string, std::unique_ptr<RayModel>> _models;
        void drawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color);
        void drawTileContent(int x, int z);
};

#endif /* !RAYLIBGUI_HPP_ */
