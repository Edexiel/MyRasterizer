#include "Light.hpp"
#include "Camera.hpp"
#include "Mat4.hpp"
#include "Tools.hpp"

Light::Light(const Vec3& position, const Colorf& lightColor, float ambientIntensity, float diffuseIntensity, float specularIntensity, float shininess) :
    _ambientIntensity{ambientIntensity},
    _diffuseIntensity{diffuseIntensity},
    _specularIntensity{specularIntensity},
    _shininess{shininess},
    _lightPosition{position},
    _lightColor{lightColor}
{
}

inline const Vec3& Light::GetPosition() const
{
    return _lightPosition;
}

void Light::SetPosition(const Vec3& position)
{
    _lightPosition = position;
}

void Light::Correct(const Mat4& view)
{
    _lightPosition = Vec4::Homogenize(view * _lightPosition);
}


inline float Light::Diffuse(const Vec3& normal, const Vec3& light_direction) const
{
    const float diffuseTerm = clamp(Vec3::DotProduct(light_direction, normal), 0.f, 1.f);

    return _diffuseIntensity * diffuseTerm; // mat_reflectance
}

inline float Light::Specular(const Vec3& normal, const Vec3& light_direction, const Vec3& camera_direction) const
{
    float specular_term{0};

    if (Vec3::DotProduct(normal, light_direction) > 0)
    {
        const Vec3 half_vect = Vec3::Normalize(light_direction + camera_direction);
        specular_term = powf(Vec3::DotProduct(normal, half_vect), _shininess);
    }
    return _specularIntensity * specular_term; // mat_specular
}

void Light::Apply(const Vec3& position, const Vec3& normal, const Vec3& CameraPosition, Color& OutColor) const
{
    // Vec3 light_pos{0, 0, 0};
    const Vec3 v_light = Vec3::Normalize(_lightPosition - position);
    const Vec3 v_camera = Vec3::Normalize(CameraPosition - position);
    const Vec3 v_normal = Vec3::Normalize(normal);

    const float diffuse = Diffuse(v_normal, v_light);
    const float specular = Specular(v_normal, v_light, v_camera);
    OutColor = OutColor * (_ambientIntensity + diffuse + specular);
    OutColor = OutColor * _lightColor;
}
