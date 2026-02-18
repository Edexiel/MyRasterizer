#include "light.hpp"

#include <cmath>

#include "color.hpp"
#include "mat4.hpp"
#include "tools.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

Light::Light(const Vec3& position,
             const Colorf& lightColor,
             float ambientIntensity,
             float diffuseIntensity,
             float specularIntensity,
             float shininess)
    : m_ambient_intensity{ambientIntensity}, m_diffuse_intensity{diffuseIntensity},
      m_specular_intensity{specularIntensity}, m_shininess{shininess}, m_light_position{position},
      m_light_color{lightColor}
{
}

inline const Vec3& Light::GetPosition() const
{
    return m_light_position;
}

void Light::SetPosition(const Vec3& position)
{
    m_light_position = position;
}

void Light::Correct(const Mat4& view)
{
    m_light_position = Vec4::Homogenize(view * m_light_position);
}

inline float Light::Diffuse(const Vec3& normal, const Vec3& lightDirection) const
{
    const float diffuseTerm = clamp(Vec3::DotProduct(lightDirection, normal), 0.F, 1.F);

    return m_diffuse_intensity * diffuseTerm; // mat_reflectance
}

inline float Light::Specular(const Vec3& normal, const Vec3& lightDirection, const Vec3& cameraDirection) const
{
    float specularTerm{0};

    if (Vec3::DotProduct(normal, lightDirection) > 0)
    {
        const Vec3 halfVect = Vec3::Normalize(lightDirection + cameraDirection);
        specularTerm = powf(Vec3::DotProduct(normal, halfVect), m_shininess);
    }
    return m_specular_intensity * specularTerm; // mat_specular
}

void Light::Apply(const Vec3& position, const Vec3& normal, const Vec3& cameraPosition, Color& outColor) const
{
    // Vec3 light_pos{0, 0, 0};
    const Vec3 vLight = Vec3::Normalize(m_light_position - position);
    const Vec3 vCamera = Vec3::Normalize(cameraPosition - position);
    const Vec3 vNormal = Vec3::Normalize(normal);

    const float diffuse = Diffuse(vNormal, vLight);
    const float specular = Specular(vNormal, vLight, vCamera);
    outColor = outColor * (m_ambient_intensity + diffuse + specular);
    outColor = outColor * m_light_color;
}