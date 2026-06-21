/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RayWindow
*/

#include "RayWindow.hpp"

RayWindow::RayWindow(int width, int height, const std::string &title)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(width, height, title.c_str());
    SetTargetFPS(60);
}

RayWindow::~RayWindow()
{
    if (IsWindowReady()) {
        CloseWindow();
    }
}

bool RayWindow::isOpen() const
{
    return !WindowShouldClose();
}

void RayWindow::beginDrawing()
{
    BeginDrawing();
}

void RayWindow::endDrawing()
{
    EndDrawing();
}

void RayWindow::clearBackground(Color color)
{
    ClearBackground(color);
}
