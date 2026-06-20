/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RayUI
*/

#ifndef RAYUI_HPP_
#define RAYUI_HPP_

#include <raylib.h>
#include <memory>
#include <string>
#include <unordered_map>
#include "Data/World.hpp"
#include "Encapsulations/RayTexture.hpp"

class RayUI {
    public:
        RayUI(World &world, const std::unordered_map<std::string, std::unique_ptr<RayTexture>> &textures);
        ~RayUI() = default;

        void drawGlobalInfo();
        void drawTileInfo(int selectedX, int selectedZ, Vector2 mousePos);

    private:
        World &_world;
        const std::unordered_map<std::string, std::unique_ptr<RayTexture>> &_textures;

        void drawResourceLine(int x, int y, const std::string &iconKey, const std::string &name, int count);
};

#endif /* !RAYUI_HPP_ */
