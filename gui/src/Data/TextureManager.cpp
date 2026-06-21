/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** TextureManager
*/

#include "TextureManager.hpp"
#include "GuiExceptions.hpp"

void TextureManager::loadAllTextures()
{
    std::string path = "gui/assets/images";

    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (!entry.is_regular_file())
                continue;
            sf::Texture texture;
            sf::Sprite sprite;
            if (!texture.loadFromFile(entry.path().string())) {
                std::cerr << "fail to load " << entry.path().string() << std::endl;
                continue;
            }
            _textures[entry.path().stem().string()] = std::move(texture);
            sprite.setTexture(_textures[entry.path().stem().string()]);
            _sprites[entry.path().stem().string()] = std::move(sprite);
        }
    } else
        throw GuiException("file assets dosen't exist");
}

const sf::Texture &TextureManager::getTexture(const std::string &key) const
{
    auto it = _textures.find(key);

    if (it == _textures.end())
        throw GuiException("Texture '" + key + "' not found in TextureManager");
    return it->second;
}

sf::Sprite &TextureManager::getSprite(const std::string &key)
{
    auto it = _sprites.find(key);

    if (it == _sprites.end())
        throw GuiException("Texture '" + key + "' not found in TextureManager");
    return it->second;
}