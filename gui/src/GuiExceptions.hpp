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


#endif /* !Gui_EXCEPTIONS_HPP_ */