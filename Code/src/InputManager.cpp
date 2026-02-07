#include "InputManager.hpp"

InputManager::InputManager()
{
    //    current_input.keys[GLFW_KEY_SPACE] = false;
    //    current_input.keys[GLFW_KEY_UP] = false;
    //    current_input.keys[GLFW_KEY_RIGHT] = false;
    //    current_input.keys[GLFW_KEY_LEFT] = false;
    //    current_input.keys[GLFW_KEY_DOWN] = false;
    //    current_input.keys[GLFW_KEY_W] = false;
    //    current_input.keys[GLFW_KEY_A] = false;
    //    current_input.keys[GLFW_KEY_S] = false;
    //    current_input.keys[GLFW_KEY_D] = false;
    //    current_input.keys[GLFW_KEY_LEFT_SHIFT] = false;
    //
    //    glfwGetCursorPos(_window, &current_input.mouse.x, &current_input.mouse.y);
    //    glfwGetCursorPos(_window, &old_input.mouse.x, &old_input.mouse.y);
}

void InputManager::Update()
{
    //    old_input = current_input;
    //
    //    // Keyboard
    //    for (const auto& key : current_input.keys)
    //        current_input.keys[key.first] = (bool)glfwGetKey(_window, key.first);
    //
    //    //Mouse keys
    //    current_input.mouse_keys.x = (bool)glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT);
    //    current_input.mouse_keys.y = (bool)glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_RIGHT);
    //
    //    //Mouse
    //    glfwGetCursorPos(_window, &current_input.mouse.x, &current_input.mouse.y);
}

inline bool InputManager::KeyUp(int key) { return old_input.keys[key] && !current_input.keys[key]; }

inline bool InputManager::KeyDown(int key) { return current_input.keys[key] && !old_input.keys[key]; }

inline bool InputManager::IsDown(int key) { return current_input.keys[key]; }

inline Vec2d InputManager::GetMousePosition() const { return current_input.mouse; }

inline Vec2d InputManager::GetMouseOldPosition() const { return old_input.mouse; }

Vec2d InputManager::GetMouseMovement() const
{
    return {current_input.mouse.x - old_input.mouse.x, current_input.mouse.y - old_input.mouse.y};
}
