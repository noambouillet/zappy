/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RayCamera
*/

#pragma once

#include <raylib.h>

class RayCamera {
    public:
        RayCamera();
        ~RayCamera() = default;

        void update(float deltaTime);
        void setTarget(float x, float z);
        void beginMode3D();
        void endMode3D();
        Camera3D &getCamera();

    private:
        Camera3D _camera;
        float _angle;
        float _radius;
        float _height;
};
