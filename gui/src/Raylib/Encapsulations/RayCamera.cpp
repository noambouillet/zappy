/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RayCamera
*/

#include "RayCamera.hpp"
#include <cmath>

RayCamera::RayCamera() : _angle(0.785f), _radius(20.0f), _height(15.0f)
{
    _camera.position = (Vector3){ 15.0f, 15.0f, 15.0f };
    _camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    _camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    _camera.fovy = 45.0f;
    _camera.projection = CAMERA_PERSPECTIVE;
}

void RayCamera::setTarget(float x, float z)
{
    _camera.target.x = x;
    _camera.target.z = z;
}

void RayCamera::update(float deltaTime)
{
    if (IsKeyDown(KEY_Q) || IsKeyDown(KEY_LEFT))
        _angle -= 2.0f * deltaTime;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        _angle += 2.0f * deltaTime;

    if (IsKeyDown(KEY_UP)) {
        _radius -= 15.0f * deltaTime;
        _height -= 10.0f * deltaTime;
    }
    if (IsKeyDown(KEY_DOWN)) {
        _radius += 15.0f * deltaTime;
        _height += 10.0f * deltaTime;
    }

    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f) {
        _radius -= wheel * 3.0f;
        _height -= wheel * 2.0f;
        if (_radius < 5.0f)
            _radius = 5.0f;
        if (_height < 2.0f)
            _height = 2.0f;
        if (_radius > 100.0f)
            _radius = 100.0f;
        if (_height > 100.0f)
            _height = 100.0f;
    }

    _camera.position.x = _camera.target.x + std::cos(_angle) * _radius;
    _camera.position.z = _camera.target.z + std::sin(_angle) * _radius;
    _camera.position.y = _camera.target.y + _height;
}

void RayCamera::beginMode3D()
{
    BeginMode3D(_camera);
}

void RayCamera::endMode3D()
{
    EndMode3D();
}

Camera3D &RayCamera::getCamera()
{
    return _camera;
}
