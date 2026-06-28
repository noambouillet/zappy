/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RayModel
*/

#include "RayModel.hpp"
#include "Exceptions/RaylibException.hpp"

RayModel::RayModel(const std::string &filepath, bool loadAnims): _animations(nullptr), _animCount(0)
{
    _model = LoadModel(filepath.c_str());
    if (_model.meshCount <= 0)
        throw RaylibException("Failed to load model from " + filepath);
        
    if (loadAnims) {
        _animations = LoadModelAnimations(filepath.c_str(), &_animCount);
    }
}

RayModel::~RayModel()
{
    if (_animations) {
        UnloadModelAnimations(_animations, _animCount);
    }
    UnloadModel(_model);
}

Model &RayModel::getModel()
{
    return _model;
}

void RayModel::updateAnimation(int animIndex, int frame, bool loop)
{
    if (_animations && animIndex >= 0 && animIndex < _animCount) {
        int maxFrame = _animations[animIndex].keyframeCount;
        int safeFrame = 0;
        if (maxFrame > 0) {
            if (loop) {
                safeFrame = frame % maxFrame;
            } else {
                safeFrame = (frame >= maxFrame) ? maxFrame - 1 : frame;
            }
            UpdateModelAnimation(_model, _animations[animIndex], safeFrame);
        }
    }
}

int RayModel::getAnimCount() const
{
    return _animCount;
}

bool RayModel::hasAnimations() const
{
    return _animations != nullptr;
}
