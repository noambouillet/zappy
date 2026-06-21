/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RayTexture
*/

#include "RayTexture.hpp"

RayTexture::RayTexture(const std::string &filePath)
{
    _texture = LoadTexture(filePath.c_str());
}

RayTexture::~RayTexture()
{
    UnloadTexture(_texture);
}

const Texture2D &RayTexture::getTexture() const
{
    return _texture;
}
