#include "scene.hpp"

#include "color.hpp"
#include "entity.hpp"
#include "light.hpp"
#include "mesh.hpp"
#include "vec3.hpp"

Scene::Scene(const InputManager& inputManager, const Camera& camera)
    : m_camera{camera}, m_im{inputManager},
      m_light{Vec3{1.F, 0.F, 0.F}, Colorf{1.F, 1.F, 1.F, 1.F}, 1.F, 0.4F, 0.4F, 20.F}
{
    // SCENE ENTITIES
    // Entity sphere{Mesh::CreateSphere(8, 16)};
    // Entity cube{Mesh::CreateCube("media/cratetex.png")};
    // // entities.push_back(Entity{Mesh::CreateCube(nullptr)});
    // cube.setPosition({0.f,0.f,-2.f});

    // entities.push_back(cube);

    Entity sphere{Mesh::CreateSphere(16, 32, Color{255, 0, 0, 255})};
    sphere.SetPosition({-1.F, 0.F, -1.5F});
    sphere.SetScale({.25f, .25f, .25f});

    Entity sphere2{Mesh::CreateSphere(3, 3, Color{200, 125, 125, 255})};
    sphere2.SetPosition({.5f, 0.F, -1.5F});
    sphere2.SetScale({.25f, .25f, .25f});
    // sphere.SetDrawMode(LINE);

    Entity triangle{Mesh::CreateTriangle()};
    triangle.SetPosition({0.F, 0.F, -1.5F});
    triangle.SetScale({1.F, 1.F, 1.F});
    // triangle.rotation = Vec3{0.f, 1.f, 0.f};
    // triangle.SetDrawMode(TRIANGLE);

    // Entity teapot{Mesh::LoadObj("media/teapot.obj")};
    // teapot.SetPosition({0.f,-2.f,-1.f});
    // teapot.SetScale({0.1f,0.1f,0.1f});

    // Entity cubeTexture{Mesh::CreateCube("Assets/crate.png")};
    Entity cube{Mesh::CreateCube(Color{255, 255, 255, 255})};
    cube.SetPosition({1.F, 0.F, -1.5F});
    cube.SetScale({0.5F, 0.5F, 0.5F});
    cube.SetRotation({2.F, 0.2F, 0.F});

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
