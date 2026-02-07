#include "Scene.hpp"
#include "Mesh.hpp"
#include "Vec3.hpp"

Scene::Scene(const InputManager& inputManager, const Camera& camera) :
    _camera{camera},
    _im{inputManager},
    _light{
        Vec3{1.f, 0.f, 0.f},
        Colorf{1.f, 1.f, 1.f, 1.f},
        1.f,
        0.4f,
        0.4f,
        20.f
    }
{
    // SCENE ENTITIES
    // Entity sphere{Mesh::CreateSphere(8, 16)};
    // Entity cube{Mesh::CreateCube("media/cratetex.png")};
    // // entities.push_back(Entity{Mesh::CreateCube(nullptr)});
    // cube.setPosition({0.f,0.f,-2.f});

    // entities.push_back(cube);

    Entity sphere{Mesh::CreateSphere(16, 32,Color{255, 0, 0, 255})};
    sphere.SetPosition({-1.f, 0.f, -1.5f});
    sphere.SetScale({.25f, .25f, .25f});

     Entity sphere2{Mesh::CreateSphere(16, 32,Color{200, 125, 125, 255})};
     sphere2.SetPosition({0.f, 0.f, 1.f});
     sphere2.SetScale({.25f, .25f, .25f});
    // sphere.SetDrawMode(LINE);

    Entity triangle{Mesh::CreateTriangle()};
    triangle.SetPosition({0.f, 0.f, -1.5f});
    triangle.SetScale({1.f, -1.f, 1.f});
    // triangle.rotation = Vec3{0.f, 1.f, 0.f};
    // triangle.SetDrawMode(TRIANGLE);


    // Entity teapot{Mesh::LoadObj("media/teapot.obj")};
    // teapot.SetPosition({0.f,-2.f,-1.f});
    // teapot.SetScale({0.1f,0.1f,0.1f});

    // Entity cubeTexture{Mesh::CreateCube("Assets/crate.png")};
    Entity cube{Mesh::CreateCube(Color{255, 255, 255, 255})};
    cube.SetPosition({1.f, 0.f, -1.5f});
    cube.SetScale({0.5f, 0.5f, 0.5f});
    cube.SetRotation({2.f, 0.2f, 0.f});

    // entities.push_back(teapot);
    entities.push_back(cube);
    entities.push_back(sphere);
    // entities.push_back(sphere2);
    entities.push_back(triangle);
    // entities.push_back(triangle);
}

Scene::~Scene()
{
}

void Scene::Update(float deltaTime)
{
    for (Entity& entity : entities)
    {
        // entity.update(deltaTime);
        entity.Transform();
    }
}

Light& Scene::GetLight() { return _light; }

const Camera& Scene::GetCamera() const { return _camera; }
