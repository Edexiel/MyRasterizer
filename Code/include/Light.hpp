#pragma once

#include "Mat4.hpp"
#include "Vec3.hpp"

#include "Color.hpp"

class Camera;

class Light
{
public:
    Light() = delete;
    Light(const Light& Light) = default;
    Light(const Vec3& position,const Colorf& light_color, float ambientIntensity, float diffuseIntensity, float specularIntensity, float shininess);

    const Vec3& GetPosition() const;
    void SetPosition(const Vec3& position);
    void Correct(const Mat4& view);

    float Diffuse(const Vec3& normal, const Vec3& light_direction) const;
    float Specular(const Vec3& normal, const Vec3& light_direction, const Vec3& camera_direction) const;
    void Apply(const Vec3& position, const Vec3& normal, const Vec3& CameraPosition, Color& color) const;

private:
    float _ambientIntensity;
    float _diffuseIntensity;
    float _specularIntensity;
    float _shininess;

    Vec3 _lightPosition;
    Colorf _lightColor;
};
