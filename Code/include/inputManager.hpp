#pragma once

#include <map>
#include "vec2.hpp"

// key to be watched and mouse
struct Input
{
    Vec2b m_mouse_keys{};
    Vec2d m_mouse{};
    std::map<int, bool> m_keys;
};

class InputManager
{
public:
    InputManager();

    void Update();
    bool KeyUp(int key);
    bool KeyDown(int key);
    bool IsDown(int key);

    Vec2d GetMousePosition() const;
    Vec2d GetMouseOldPosition() const;
    Vec2d GetMouseMovement() const;
private:
    Input m_current_input;
    Input m_old_input;
};
