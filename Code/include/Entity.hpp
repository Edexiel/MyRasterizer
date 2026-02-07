#pragma once

#include <memory>

#include "Mat4.hpp"
#include "Mesh.hpp"
#include "Tools.hpp"
#include "Vec3.hpp"

class Entity
{
public:
    Entity();
    explicit Entity(const std::shared_ptr<Mesh>& Mesh);
    // Entity(Entity& other);
    // Entity(const Entity& other);
    // Entity(Entity&& other) noexcept = default;
    // Entity& operator=(const Entity& other) = default;
    // Entity& operator=(Entity&& other) noexcept = default;

    void Update(double deltaTime);
    void Transform();

    void ResetTransformation();

    void SetDrawMode(E_DRAW_MODE draw_mode);
    E_DRAW_MODE GetDrawMode() const;

    const Vec3& GetPosition() const;
    void SetPosition(const Vec3& position);

    const Vec3& GetRotation() const;
    void SetRotation(const Vec3& rotation);

    const Vec3& GetScale() const;
    void SetScale(const Vec3& scale);

    const std::shared_ptr<Mesh>& GetMesh() const;
    void SetMesh(const std::shared_ptr<Mesh>& mesh);

    const Mat4& GetTransform() const;
    void SetTransform(const Mat4& transform);

private:
    E_DRAW_MODE _drawMode;
    Vec3 _position{};
    Vec3 _rotation{};
    Vec3 _scale{};

    std::shared_ptr<Mesh> _mesh;
    Mat4 _transform{};
};
