#pragma once

#include "InputManager.hpp"
#include "Mat4.hpp"
#include "Vec3.hpp"

class Camera
{
public:
    Camera() = delete;
    Camera(const InputManager& inputManager, Vec3 position,Vec3 direction, float pitch, float yaw, float mouse_speed,
           float movement_speed);

    Vec3 GetPosition() const;
    void Update(float deltaTime);
    Mat4 GetCameraMatrix() const;
private:
    const InputManager& _inputManager;
    float _mouse_speed;
    float _movement_speed;
    float _pitch;
    float _yaw;
    Vec3 _position;
    Vec3 _direction;

};
