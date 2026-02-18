#include "camera.hpp"

#include "inputManager.hpp"
#include "mat4.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

Camera::Camera(const InputManager& inputManager,
               Vec3 position,
               Vec3 direction,
               float pitch,
               float yaw,
               float mouseSpeed,
               float movementSpeed)
    : m_input_manager{inputManager}, m_mouse_speed{mouseSpeed}, m_movement_speed{movementSpeed}, m_pitch{pitch},
      m_yaw{yaw}, m_position{position}, m_direction{direction}
{
}

void Camera::Update(float deltaTime)
{
    const Vec2d mouse = m_input_manager.GetMouseMovement();

    m_pitch += (float) mouse.y * m_mouse_speed;
    m_pitch = std::min(m_pitch, 90.f);
    m_pitch = std::max(m_pitch, -90.f);

    m_yaw += (float) mouse.x * m_mouse_speed;
    if (m_yaw > 180.F)
    {
        m_yaw = -180.F;
    }
    if (m_yaw < -180.F)
    {
        m_yaw = 180.F;
    }

    m_direction.x = sinf(m_yaw * std::numbers::pi_v<float> / 180.F) * m_movement_speed * deltaTime;
    m_direction.z = -cosf(m_yaw * std::numbers::pi_v<float> / 180.F) * m_movement_speed * deltaTime;
    m_direction.y = 0;

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

Vec3 Camera::GetPosition() const
{
    return m_position;
}

Mat4 Camera::GetCameraMatrix() const
{
    return Mat4::CreateXRotationMatrix(m_pitch * std::numbers::pi_v<float> / 180.F) *
           Mat4::CreateYRotationMatrix(m_yaw * std::numbers::pi_v<float> / 180.F) *
           Mat4::CreateTranslationMatrix(m_position);
}