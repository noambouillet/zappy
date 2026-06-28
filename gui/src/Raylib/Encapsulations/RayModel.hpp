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
        RayModel(const std::string &filepath, bool loadAnims = false);
        ~RayModel();

        Model &getModel();
        void updateAnimation(int animIndex, int frame, bool loop = true);
        int getAnimCount() const;
        bool hasAnimations() const;

    private:
        Model _model;
        ModelAnimation *_animations;
        int _animCount;
};

#endif /* !RAYMODEL_HPP_ */
