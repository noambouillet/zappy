/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RayTexture
*/

#pragma once

#include <raylib.h>
#include <string>

class RayTexture {
    public:
        RayTexture(const std::string &filePath);
        ~RayTexture();

        RayTexture(const RayTexture &) = delete;
        RayTexture &operator=(const RayTexture &) = delete;

        const Texture2D &getTexture() const;

    private:
        Texture2D _texture;
};
