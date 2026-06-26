/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RayModel
*/

#ifndef RAYMODEL_HPP_
#define RAYMODEL_HPP_

#include <raylib.h>
#include <string>

class RayModel {
    public:
        RayModel(const std::string &filepath);
        ~RayModel();

        Model &getModel();

    private:
        Model _model;
};

#endif /* !RAYMODEL_HPP_ */
