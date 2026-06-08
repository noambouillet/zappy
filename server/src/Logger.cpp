/*
** EPITECH PROJECT, 2026
** Logger
** File description:
** Singleton representing the logger
*/

#include "Logger.hpp"

namespace ZappyServer {

Logger::Logger(const std::string &path)
{
    _file.open(path, std::ofstream::out | std::ofstream::app);

    if (!_file.is_open())
        std::cerr << "Error: cannot open log file." << std::endl;
    if (std::filesystem::exists(path) && std::filesystem::file_size(path) > 0)
        _file << '\n';
    std::time_t now = std::time(nullptr);
    std::string date = std::ctime(&now);
    if (!date.empty() && date.back() == '\n')
        date.pop_back();
    _file << date << ". The world has been awakened." << std::endl;
}

Logger::~Logger()
{
}

void Logger::write(const std::string &message)
{
    if (_file.is_open())
        _file << message << std::endl;
}

Logger logger("server.log");

}
