#pragma once

#include <vector>
#include "Entity.hpp"
#include "Light.hpp"

class Camera;
class InputManager;

class Scene
{
private:
    const Camera& _camera;
    const InputManager& _im;

    Light _light;

public:
    Scene(const InputManager& im, const Camera& camera);
    ~Scene();

    std::vector<Entity> entities;

    Light& GetLight();

    const Camera& GetCamera() const;
    void Update(float deltaTime);
};
