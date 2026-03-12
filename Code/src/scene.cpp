#include "scene.hpp"

#include "color.hpp"
#include "entity.hpp"
#include "light.hpp"
#include "mesh.hpp"
#include "vec3.hpp"

Scene::Scene(const InputManager& inputManager, const Camera& camera)
    : m_camera{camera},
      m_im{inputManager}
{
    // SCENE ENTITIES
    // Entity sphere{Mesh::CreateSphere(8, 16)};
    // Entity cube{Mesh::CreateCube("media/cratetex.png")};
    // // entities.push_back(Entity{Mesh::CreateCube(nullptr)});
    // cube.setPosition({0.f,0.f,-2.f});

    // entities.push_back(cube);

    Light mainLight {
        Vec3{2.f, 2.f, 2.f},
        Colors::White,
        1.f, 0.4f, 0.4f, 20.f};

    Entity sphere{Mesh::CreateSphere(16, 16, Colors::Red)};
    sphere.SetPosition({-1.f, 0.f, 1.5f});
    sphere.SetScale({.25f, .25f, .25f});

    Entity sphere2{Mesh::CreateSphere(2, 4, Color{20, 200, 20, 255})};
    sphere2.SetPosition({.5f, 0.f, 1.5f});
    sphere2.SetScale({.25f, .5f, .25f});

    Entity triangle{Mesh::CreateTriangle()};
    triangle.SetPosition({0.f, 0.f, 1.5f});
    triangle.SetScale({2.f, 2.f, 2.f});
    // triangle.rotation = Vec3{0.f, 1.f, 0.f};
    // triangle.SetDrawMode(TRIANGLE);

    // Entity teapot{Mesh::LoadObj("media/teapot.obj")};
    // teapot.SetPosition({0.f,-2.f,-1.f});
    // teapot.SetScale({0.1f,0.1f,0.1f});

    Entity cubeTexture{Mesh::CreateTexCube("Assets/crate.png")};
    // Entity cube{Mesh::CreateCube(Color{255, 255, 255, 255})};
    cubeTexture.SetPosition({0.f, 0.f, 1.5f});
    cubeTexture.SetScale({0.5f, 0.5f, 0.5f});
    cubeTexture.SetRotation({2.f, 0.2f, 0.f});

    m_lights.push_back(mainLight);
    // entities.push_back(teapot);
    m_entities.push_back(cubeTexture);
    m_entities.push_back(sphere);
    m_entities.push_back(sphere2);
    m_entities.push_back(triangle);
    // entities.push_back(triangle);
}

Scene::~Scene() = default;

std::vector<Light>& Scene::GetLights()
{
    return m_lights;
}

std::vector<Entity>& Scene::GetEntities()
{
    return m_entities;
}

void Scene::Update(float deltaTime)
{
    for (Entity& entity : m_entities)
    {
        entity.Update(deltaTime);
        entity.Transform();
    }
}

const Camera& Scene::GetCamera() const
{
    return m_camera;
}
