/*
** EPITECH PROJECT, 2026
** GuiExceptions
** File description:
** CLass that contains the error messages of the zappy Gui
*/

#ifndef GUI_EXCEPTIONS_HPP_
#define GUI_EXCEPTIONS_HPP_

#include <exception>
#include <string>

class GuiException : public std::exception {
    private:
        std::string _message;
    public:
        explicit GuiException(std::string message) noexcept : _message(std::move(message)) {}

        const char *what() const noexcept override
        {
            return _message.c_str();
        }
};

class MinorGuiException : public GuiException {
    public:
        explicit MinorGuiException(std::string message) noexcept : GuiException(std::move(message)) {}
};

class MajorGuiException : public GuiException {
    public:
        explicit MajorGuiException(std::string message) noexcept : GuiException(std::move(message)) {}
};

class ParsingGuiException : public MajorGuiException {
    public:
        explicit ParsingGuiException(std::string message) noexcept : MajorGuiException(std::move(message)) {}
};

class NetworkException : public MajorGuiException {
    public:
        explicit NetworkException(std::string message) noexcept : MajorGuiException(std::move(message)) {}
};

class MinorNetworkException : public MinorGuiException {
    public:
        explicit MinorNetworkException(std::string message) noexcept : MinorGuiException(std::move(message)) {}
};

#endif /* !GUI_EXCEPTIONS_HPP_ */