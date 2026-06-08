/*
** EPITECH PROJECT, 2026
** Logger
** File description:
** Singleton representing the logger
*/

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <fstream>
#include <string>
#include <ctime>
#include <iostream>
#include <filesystem>

namespace ZappyServer {

class Logger {
    private:
        std::ofstream _file;
    public:
        Logger(const std::string &path);
        ~Logger();

        void write(const std::string &message);
};

extern Logger logger;

}

#endif /* !LOGGER_HPP_ */
