#pragma once

#include <vector>
#include "entity.hpp"
#include "light.hpp"

class Camera;
class InputManager;

class Scene
{
public:
    Scene(const InputManager& inputManager, const Camera& camera);
    ~Scene();


    std::vector<Light>& GetLights();
    std::vector<Entity>& GetEntities();

    const Camera& GetCamera() const;
    void Update(float deltaTime);
private:
    const Camera& m_camera;
    const InputManager& m_im;

    std::vector<Entity> m_entities;
    std::vector<Light> m_lights;
};
