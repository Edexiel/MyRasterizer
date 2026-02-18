#pragma once

#include <memory>

#include "mat4.hpp"
#include "mesh.hpp"
#include "tools.hpp"
#include "vec3.hpp"

class Entity
{
public:
    Entity();
    explicit Entity(const std::shared_ptr<Mesh>& mesh);

    static void Update(double deltaTime);
    void Transform();

    void ResetTransformation();

    void SetDrawMode(E_DRAW_MODE drawMode);
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
    E_DRAW_MODE m_draw_mode;
    Vec3 m_position;
    Vec3 m_rotation;
    Vec3 m_scale;

    std::shared_ptr<Mesh> m_mesh;
    Mat4 m_transform;
};
