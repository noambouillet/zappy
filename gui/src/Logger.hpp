/*
** EPITECH PROJECT, 2026
** Logger
** File description:
** Singleton representing the logger for the GUI
*/

#ifndef GUI_LOGGER_HPP_
#define GUI_LOGGER_HPP_

#include <string>
#include <iostream>
#include <ctime>

class GuiLogger {
    private:
        bool _verbose = false;
    public:
        GuiLogger();
        ~GuiLogger();

        void setVerbose(bool verbose);
        void write(const std::string &message);
        void info(const std::string &message);
        void warn(const std::string &message);
        void error(const std::string &message);
};

extern GuiLogger logger;

#endif /* !GUI_LOGGER_HPP_ */
