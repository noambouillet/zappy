/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RaylibException
*/

#ifndef RAYLIB_EXCEPTION_HPP_
#define RAYLIB_EXCEPTION_HPP_

#include "GuiExceptions.hpp"

class RaylibException : public GuiException {
    public:
        explicit RaylibException(const std::string &message) noexcept : GuiException("Raylib Error: " + message) {}
};

#endif /* !RAYLIB_EXCEPTION_HPP_ */
