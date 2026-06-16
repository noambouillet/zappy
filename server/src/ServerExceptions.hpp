/*
** EPITECH PROJECT, 2026
** ServerExceptions
** File description:
** CLass that contains the error messages of the zappy server
*/

#ifndef SERVER_EXCEPTIONS_HPP_
#define SERVER_EXCEPTIONS_HPP_

#include <exception>
#include <string>

namespace ZappyServer {

class ServerException : public std::exception {
    private:
        std::string _message;
    public:
        explicit ServerException(std::string message) noexcept : _message(std::move(message)) {}

        const char *what() const noexcept override
        {
            return _message.c_str();
        }
};

}

#endif /* !SERVER_EXCEPTIONS_HPP_ */
