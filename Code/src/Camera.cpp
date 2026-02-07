#include "Camera.hpp"
#include <iostream>
#include "InputManager.hpp"
#include "cmath"
#include "Vec2.hpp"

Camera::Camera(const InputManager& inputManager, Vec3 position,Vec3 direction, float pitch, float yaw, float mouse_speed,
               float movement_speed) :
                                    _inputManager{inputManager},
                                    _mouse_speed{mouse_speed},
                                    _movement_speed{movement_speed},
                                    _pitch{pitch},
                                    _yaw{yaw},
                                    _position{position},
                                     _direction{direction}
{
}

void Camera::Update(float deltaTime)
{
    const Vec2d mouse = _inputManager.GetMouseMovement();

    _pitch += (float)mouse.y * _mouse_speed;
    if (_pitch > 90.f)
        _pitch = 90.f;
    if (_pitch < -90.f)
        _pitch = -90.f;

    _yaw += (float)mouse.x * _mouse_speed;
    if (_yaw > 180.f)
        _yaw = -180.f;
    if (_yaw < -180.f)
        _yaw = 180.f;

    _direction.x = sinf(_yaw * (float)M_PI / 180.f) * _movement_speed * deltaTime;
    _direction.z = -cosf(_yaw * (float)M_PI / 180.f) * _movement_speed * deltaTime;
    _direction.y = 0;

    // if (_input->IsDown(GLFW_KEY_UP) || _input->IsDown(GLFW_KEY_W))
    // {
    //     _position = _position - _direction;
    // }
    // else if (_input->IsDown(GLFW_KEY_DOWN) || _input->IsDown(GLFW_KEY_S))
    // {
    //     _position = _position + _direction;
    // }
    //
    // if (_input->IsDown(GLFW_KEY_RIGHT) || _input->IsDown(GLFW_KEY_D))
    // {
    //     _position.x += _direction.z;
    //     _position.z -= _direction.x;
    // }
    // else if (_input->IsDown(GLFW_KEY_LEFT) || _input->IsDown(GLFW_KEY_A))
    // {
    //     _position.x -= _direction.z;
    //     _position.z += _direction.x;
    // }
    //
    // if (_input->IsDown(GLFW_KEY_SPACE))
    // {
    //     _position.y -= _movement_speed * deltaTime;
    // }
    // else if (_input->IsDown(GLFW_KEY_LEFT_SHIFT))
    // {
    //     _position.y += _movement_speed * deltaTime;
    // }
}

Vec3 Camera::GetPosition() const { return _position; }

Mat4 Camera::GetCameraMatrix() const
{
    return Mat4::CreateXRotationMatrix(_pitch * (float)M_PI / 180.f) *
        Mat4::CreateYRotationMatrix(_yaw * (float)M_PI / 180.f) * Mat4::CreateTranslationMatrix(_position);
}
