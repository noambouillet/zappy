/*
** EPITECH PROJECT, 2026
** Logger
** File description:
** Singleton representing the logger for the GUI
*/

#include "Logger.hpp"

GuiLogger::GuiLogger()
{
}

GuiLogger::~GuiLogger()
{
}

void GuiLogger::setVerbose(bool verbose)
{
    _verbose = verbose;
}

void GuiLogger::write(const std::string &message)
{
    std::time_t now = std::time(nullptr);
    std::string date = std::ctime(&now);
    if (!date.empty() && date.back() == '\n') {
        date.pop_back();
    }
    std::cout << "[" << date << "] " << message << std::endl;
}

void GuiLogger::info(const std::string &message)
{
    if (!_verbose)
        return;
    write("[INFO] " + message);
}

void GuiLogger::warn(const std::string &message)
{
    if (!_verbose)
        return;
    write("[WARN] " + message);
}

void GuiLogger::error(const std::string &message)
{
    write("[ERROR] " + message);
}

GuiLogger logger;
