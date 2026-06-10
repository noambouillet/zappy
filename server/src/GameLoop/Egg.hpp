/*
** EPITECH PROJECT, 2026
** Egg
** File description:
** Egg structure for player reproduction
*/

#ifndef EGG_HPP_
#define EGG_HPP_

#include <string>

namespace ZappyServer {

struct Egg {
    unsigned int id;
    std::string teamName;
    unsigned int x;
    unsigned int y;
};

}

#endif /* !EGG_HPP_ */
