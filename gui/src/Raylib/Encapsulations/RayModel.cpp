/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RayModel
*/

#include "RayModel.hpp"

RayModel::RayModel(const std::string &filepath)
{
    _model = LoadModel(filepath.c_str());
}

RayModel::~RayModel()
{
    UnloadModel(_model);
}

Model &RayModel::getModel()
{
    return _model;
}
