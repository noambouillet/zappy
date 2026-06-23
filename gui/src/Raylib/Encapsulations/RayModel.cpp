/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RayModel
*/

#include "RayModel.hpp"
#include "Exceptions/RaylibException.hpp"

RayModel::RayModel(const std::string &filepath)
{
    _model = LoadModel(filepath.c_str());
    if (_model.meshCount <= 0)
        throw RaylibException("Failed to load model from " + filepath);
}

RayModel::~RayModel()
{
    UnloadModel(_model);
}

Model &RayModel::getModel()
{
    return _model;
}
