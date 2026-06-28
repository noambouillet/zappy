/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RayWindow
*/

#ifndef RAYWINDOW_HPP_
#define RAYWINDOW_HPP_

#include <string>
#include <raylib.h>

class RayWindow {
    public:
        RayWindow(int width, int height, const std::string &title);
        ~RayWindow();

        RayWindow(const RayWindow &) = delete;
        RayWindow &operator=(const RayWindow &) = delete;

        bool isOpen() const;
        void show();
        void beginDrawing();
        void endDrawing();
        void clearBackground(Color color);
};

#endif /* !RAYWINDOW_HPP_ */
