#pragma once
#include "vec3.hpp"

class Vec4
{
public:
    union
    {
        struct
        {
            float x = 0.f;
            float y = 0.f;
            float z = 0.f;
            float w = 0.f;
        };
        float array[4];
    };

    Vec4() = default;
    Vec4(float x, float y, float z, float w);
    Vec4(const Vec3& vec3, float w);

    Vec4& Homogenize();
    static Vec3 Homogenize(const Vec4& v);
    void Normalize();
    Vec4 operator+(const Vec4& other) const;
    Vec4 operator*(float other) const;

    float& operator[](int i);
    const float& operator[](int i) const;
};

inline Vec4::Vec4(float x, float y, float z, float w) : x{x}, y{y}, z{z}, w{w}
{
}
inline Vec4::Vec4(const Vec3& vec3, float w = 1.0f)
{
    this->x = vec3.x;
    this->y = vec3.y;
    this->z = vec3.z;
    this->w = w;
}

inline Vec4& Vec4::Homogenize()
{
    if (w == 0 || w == 1)
    {
        return *this;
    }

    x /= w;
    y /= w;
    z /= w;

    return *this;
}

inline Vec3 Vec4::Homogenize(const Vec4& v)
{
    if (v.w == 0 || v.w == 1)
    {
        return Vec3{v.x, v.y, v.z};
    }

    const Vec3 result{v.x / v.w, v.y / v.w, v.z / v.w};

    return result;
}

inline Vec4 Vec4::operator+(const Vec4& other) const {
    return {x + other.x, y + other.y, z + other.z, w + other.w};
}

inline Vec4 Vec4::operator*(float other) const
{
    return {x * other, y * other, z * other, w * other};
}

inline float& Vec4::operator[](const int i)
{
    return array[i];
}

inline const float& Vec4::operator[](const int i) const
{
    return array[i];
}
