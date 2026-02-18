#pragma once

#include "inputManager.hpp"
#include "mat4.hpp"
#include "vec3.hpp"

class Camera
{
public:
    Camera() = delete;
    Camera(const InputManager& inputManager,
           Vec3 position,
           Vec3 direction,
           float pitch,
           float yaw,
           float mouseSpeed,
           float movementSpeed);

    Vec3 GetPosition() const;
    void Update(float deltaTime);
    Mat4 GetCameraMatrix() const;

private:
    const InputManager& m_input_manager;
    float m_mouse_speed;
    float m_movement_speed;
    float m_pitch{};
    float m_yaw{};
    Vec3 m_position;
    Vec3 m_direction;
};