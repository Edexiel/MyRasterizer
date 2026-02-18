#include "entity.hpp"

#include <memory>

#include "mat4.hpp"
#include "mesh.hpp"
#include "tools.hpp"
#include "vec3.hpp"

Entity::Entity()
    : m_draw_mode{E_DRAW_MODE::triangle}, m_position{Vec3::Zero()}, m_rotation{0.F, 0.F, 0.F}, m_scale{1.F, 1.F, 1.F}, m_mesh{nullptr},
      m_transform{Mat4::Identity()}
{
}

Entity::Entity(const std::shared_ptr<Mesh>& mesh)
    : m_draw_mode{E_DRAW_MODE::triangle}, m_position{0.F, 0.F, 0.F}, m_rotation{0.F, 0.F, 0.F}, m_scale{1.F, 1.F, 1.F}, m_mesh{mesh},
      m_transform{Mat4::Identity()}
{
}

// Entity::Entity(Entity& other) : _drawMode(other._drawMode),_position(other._position),_rotation(other._rotation),_scale(other._scale),_mesh(std::move(other._mesh))
// {
// }
//
// Entity::Entity(const Entity& other): _drawMode(other._drawMode),_position(other._position),_rotation(other._rotation),_scale(other._scale),_mesh(std::move(other._mesh))
// {
//
// }

// Entity::Entity(const Entity& other) : _drawMode(other._drawMode),_position(other._position),_rotation(other._rotation),_scale(other._scale),_mesh(other._mesh)
// {
// }
//
// Entity::Entity(Entity&& other) noexcept : _drawMode(other._drawMode),
//                                           _position(other._position),
//                                           _rotation(other._rotation),
//                                           _scale(other._scale),
//                                           _transform(other._transform),
// _mesh(other._mesh)
// {
// }

void Entity::Update(const double deltaTime)
{
    (void) deltaTime;
}

void Entity::Transform()
{
    m_transform = Mat4::CreateTransformMatrix(m_position, m_rotation, m_scale);
}

void Entity::ResetTransformation()
{
    m_transform = Mat4::Identity();
    // memset(&_transform, 0, 16 * sizeof(float));
    // _transform.a[0] = 1.f;
    // _transform.a[5] = 1.f;
    // _transform.a[10] = 1.f;
    // _transform.a[15] = 1.f;
}

void Entity::SetDrawMode(E_DRAW_MODE drawMode)
{
    m_draw_mode = drawMode;
}

E_DRAW_MODE Entity::GetDrawMode() const
{
    return m_draw_mode;
}

const Vec3& Entity::GetPosition() const
{
    return m_position;
}

void Entity::SetPosition(const Vec3& position)
{
    m_position = position;
}

const Vec3& Entity::GetRotation() const
{
    return m_rotation;
}

void Entity::SetRotation(const Vec3& rotation)
{
    m_rotation = rotation;
}

const Vec3& Entity::GetScale() const
{
    return m_scale;
}

void Entity::SetScale(const Vec3& scale)
{
    m_scale = scale;
}

const std::shared_ptr<Mesh>& Entity::GetMesh() const
{
    return m_mesh;
}

void Entity::SetMesh(const std::shared_ptr<Mesh>& mesh)
{
    m_mesh = mesh;
}

const Mat4& Entity::GetTransform() const
{
    return m_transform;
}

void Entity::SetTransform(const Mat4& transform)
{
    m_transform = transform;
}
