#pragma once

#include "mat4.hpp"
#include "vec3.hpp"

#include "color.hpp"

class Camera;

class Light
{
public:
    Light() = delete;
    Light(const Vec3& position,
          const Colorf& lightColor,
          float ambientIntensity,
          float diffuseIntensity,
          float specularIntensity,
          float shininess);

    const Vec3& GetPosition() const;
    void SetPosition(const Vec3& position);
    void Correct(const Mat4& view);

    float Diffuse(const Vec3& normal, const Vec3& lightDirection) const;
    float Specular(const Vec3& normal, const Vec3& lightDirection, const Vec3& cameraDirection) const;
    void Apply(const Vec3& position, const Vec3& normal, const Vec3& cameraPosition, Color& outColor) const;

private:
    float m_ambient_intensity;
    float m_diffuse_intensity;
    float m_specular_intensity;
    float m_shininess;

    Vec3 m_light_position;
    Colorf m_light_color;
};
