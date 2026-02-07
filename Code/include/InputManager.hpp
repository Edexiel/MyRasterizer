#pragma once

#include <map>
#include "Vec2.hpp"

// key to be watched and mouse
struct input
{
    Vec2b mouse_keys;
    Vec2d mouse;
    std::map<int, bool> keys;
};

class InputManager
{
private:
    input current_input;
    input old_input;

public:
    InputManager();

    void Update();
    bool KeyUp(int key);
    bool KeyDown(int key);
    bool IsDown(int key);

    Vec2d GetMousePosition() const;
    Vec2d GetMouseOldPosition() const;
    Vec2d GetMouseMovement() const;
};
