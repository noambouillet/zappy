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

class MinorServerException : public ServerException {
    public:
        explicit MinorServerException(std::string message) noexcept : ServerException(std::move(message)) {}
};

class MajorServerException : public ServerException {
    public:
        explicit MajorServerException(std::string message) noexcept : ServerException(std::move(message)) {}
};

class SocketException : public MinorServerException {
    public:
        explicit SocketException(std::string message) noexcept : MinorServerException(std::move(message)) {}
};

class FatalSocketException : public MajorServerException {
    public:
        explicit FatalSocketException(std::string message) noexcept : MajorServerException(std::move(message)) {}
};

class PollException : public MinorServerException {
    public:
        explicit PollException(std::string message) noexcept : MinorServerException(std::move(message)) {}
};

class FatalPollException : public MajorServerException {
    public:
        explicit FatalPollException(std::string message) noexcept : MajorServerException(std::move(message)) {}
};

class ParsingException : public MajorServerException {
    public:
        explicit ParsingException(std::string message) noexcept : MajorServerException(std::move(message)) {}
};

}

#endif /* !SERVER_EXCEPTIONS_HPP_ */
