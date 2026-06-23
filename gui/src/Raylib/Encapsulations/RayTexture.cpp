/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RayTexture
*/

#include "RayTexture.hpp"
#include "Exceptions/RaylibException.hpp"

RayTexture::RayTexture(const std::string &filePath)
{
    _texture = LoadTexture(filePath.c_str());
    if (_texture.id <= 0)
        throw RaylibException("Failed to load texture from " + filePath);
}

RayTexture::~RayTexture()
{
    UnloadTexture(_texture);
}

const Texture2D &RayTexture::getTexture() const
{
    return _texture;
}
