#include "Entity.hpp"

Entity::Entity() : _drawMode{TRIANGLE},
                   _position{Vec3::Zero()},
                   _rotation{0.f, 0.f, 0.f},
                   _scale{1.f, 1.f, 1.f},
                   _mesh{nullptr},
                   _transform{Mat4::Identity()}
{
}

Entity::Entity(const std::shared_ptr<Mesh>& Mesh) : _drawMode{TRIANGLE},
                             _position{0.f, 0.f, 0.f},
                             _rotation{0.f, 0.f, 0.f},
                             _scale{1.f, 1.f, 1.f},
                             _mesh{std::move(Mesh)},
                             _transform{Mat4::Identity()}
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
    (void)deltaTime;
}

void Entity::Transform()
{
    _transform = Mat4::CreateTransformMatrix(_position, _rotation, _scale);
}


void Entity::ResetTransformation()
{
    _transform = Mat4::Identity();
    // memset(&_transform, 0, 16 * sizeof(float));
    // _transform.a[0] = 1.f;
    // _transform.a[5] = 1.f;
    // _transform.a[10] = 1.f;
    // _transform.a[15] = 1.f;
}

void Entity::SetDrawMode(E_DRAW_MODE draw_mode) { _drawMode = draw_mode; }

E_DRAW_MODE Entity::GetDrawMode() const { return _drawMode; }

const Vec3& Entity::GetPosition() const
{
    return _position;
}

void Entity::SetPosition(const Vec3& position)
{
    _position = position;
}

const Vec3& Entity::GetRotation() const
{
    return _rotation;
}

void Entity::SetRotation(const Vec3& rotation)
{
    _rotation = rotation;
}

const Vec3& Entity::GetScale() const
{
    return _scale;
}

void Entity::SetScale(const Vec3& scale)
{
    _scale = scale;
}

const std::shared_ptr<Mesh>& Entity::GetMesh() const
{
    return _mesh;
}

void Entity::SetMesh(const std::shared_ptr<Mesh>& mesh)
{
    _mesh = std::move(mesh);
}

const Mat4& Entity::GetTransform() const
{
    return _transform;
}

void Entity::SetTransform(const Mat4& transform)
{
    _transform = transform;
}
