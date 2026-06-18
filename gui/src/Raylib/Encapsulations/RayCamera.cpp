/*
** EPITECH PROJECT, 2026
** zappy-mirror
** File description:
** RayCamera
*/

#include "RayCamera.hpp"
#include <cmath>

RayCamera::RayCamera() : _yaw(0.785f), _pitch(0.785f), _radius(20.0f)
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
        _yaw -= 2.0f * deltaTime;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        _yaw += 2.0f * deltaTime;

    if (IsKeyDown(KEY_UP))
        _radius -= 15.0f * deltaTime;
    if (IsKeyDown(KEY_DOWN))
        _radius += 15.0f * deltaTime;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 mouseDelta = GetMouseDelta();
        _yaw += mouseDelta.x * 0.005f;
        _pitch += mouseDelta.y * 0.005f;
    }

    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f)
        _radius -= wheel * 3.0f;
    if (_radius < 5.0f)
        _radius = 5.0f;
    if (_radius > 60.0f)
        _radius = 60.0f;
    if (_pitch > 1.5f)
        _pitch = 1.5f;
    if (_pitch < -1.5f)
        _pitch = -1.5f;

    _camera.position.x = _camera.target.x + _radius * std::cos(_pitch) * std::cos(_yaw);
    _camera.position.y = _camera.target.y + _radius * std::sin(_pitch);
    _camera.position.z = _camera.target.z + _radius * std::cos(_pitch) * std::sin(_yaw);
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
