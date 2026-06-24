/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** UIRender
*/

#ifndef UIRENDER_HPP_
#define UIRENDER_HPP_
#include "World.hpp"
#include <SFML/Graphics.hpp>
#include "TextureManager.hpp"

class UIRender {
    public:
        UIRender(World &world, sf::RenderWindow &window, TextureManager &textureManager);
        ~UIRender();
        void displayUI();
        void handleEvent(const sf::Event &event);
    private:
        World &_world;
        sf::RenderWindow &_window;
        TextureManager &_textureManager;
        sf::Font _font;
        sf::Text _text;
        sf::RectangleShape _buttonShape;
        std::string _selectedTeam = "";
        std::string _hoveredTeam = "";
        int _activeDataTrantorianId;

        void displayRessources();
        void displayTeams();
        sf::FloatRect drawButton(std::string text, int fontSize, float x, float y, float width, float height);
        void drawText(std::string text, int size, float x, float y);
        void drawIcon(std::string textureKey, float size, float x, float y);
        std::vector<Trantorian_t> getTeamTrantorians(const std::string &teamName) const;
        void displayTrantorianData(int trantorianId);
        void drawPanel(float x, float y, float width, float height);
        void displayTileData(int x, int y);
        void displayGameOver();
        void displayLevelLegend();

};
#endif