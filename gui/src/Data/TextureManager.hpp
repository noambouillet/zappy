/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** TextureManager
*/

#ifndef TEXTUREMANAGER_HPP_
#define TEXTUREMANAGER_HPP_

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <iostream>

class TextureManager {
    public:
        TextureManager() = default;
        ~TextureManager() = default;
        void loadAllTextures();
        const sf::Texture &getTexture(const std::string &key) const;
        sf::Sprite &getSprite(const std::string &key);
    private:
        std::unordered_map<std::string, sf::Texture> _textures;
        std::unordered_map<std::string, sf::Sprite> _sprites;
};


#endif /* !TEXTUREMANAGER_HPP_ */
