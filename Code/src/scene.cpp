#include "scene.hpp"

#include "color.hpp"
#include "entity.hpp"
#include "light.hpp"
#include "mesh.hpp"
#include "vec3.hpp"

Scene::Scene(const InputManager& inputManager, const Camera& camera)
    : m_camera{camera},
      m_im{inputManager},
      m_light{Vec3{1.f, 0.f, 0.f}, Colorf{1.f, 1.f, 1.f, 1.f}, 1.f, 0.4f, 0.4f, 20.f}
{
    // SCENE ENTITIES
    // Entity sphere{Mesh::CreateSphere(8, 16)};
    // Entity cube{Mesh::CreateCube("media/cratetex.png")};
    // // entities.push_back(Entity{Mesh::CreateCube(nullptr)});
    // cube.setPosition({0.f,0.f,-2.f});

    // entities.push_back(cube);

    Entity sphere{Mesh::CreateSphere(16, 32, Color{255, 0, 0, 255})};
    sphere.SetPosition({-1.f, 0.f, 1.5f});
    sphere.SetScale({.25f, .25f, .25f});

    Entity sphere2{Mesh::CreateSphere(3, 3, Color{200, 125, 125, 255})};
    sphere2.SetPosition({.5f, 0.f, 1.5f});
    sphere2.SetScale({.25f, .25f, .25f});

    Entity triangle{Mesh::CreateTriangle()};
    triangle.SetPosition({0.f, 0.f, 1.5f});
    triangle.SetScale({1.f, 1.f, 1.f});
    // triangle.rotation = Vec3{0.f, 1.f, 0.f};
    // triangle.SetDrawMode(TRIANGLE);

    // Entity teapot{Mesh::LoadObj("media/teapot.obj")};
    // teapot.SetPosition({0.f,-2.f,-1.f});
    // teapot.SetScale({0.1f,0.1f,0.1f});

    // Entity cubeTexture{Mesh::CreateCube("Assets/crate.png")};
    Entity cube{Mesh::CreateCube(Color{255, 255, 255, 255})};
    cube.SetPosition({1.f, 0.f, 1.5f});
    cube.SetScale({0.5f, 0.5f, 0.5f});
    cube.SetRotation({2.f, 0.2f, 0.f});

    // entities.push_back(teapot);
    m_entities.push_back(cube);
    m_entities.push_back(sphere);
    m_entities.push_back(sphere2);
    m_entities.push_back(triangle);
    // entities.push_back(triangle);
}

Scene::~Scene() = default;

void Scene::Update(float /*deltaTime*/)
{
    for (Entity& entity : m_entities)
    {
        // entity.update(deltaTime);
        entity.Transform();
    }
}

Light& Scene::GetLight()
{
    return m_light;
}

const Camera& Scene::GetCamera() const
{
    return m_camera;
}
