/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RayTexture
*/

#ifndef RAYTEXTURE_HPP_
#define RAYTEXTURE_HPP_

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

#endif /* !RAYTEXTURE_HPP_ */
