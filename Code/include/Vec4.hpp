#pragma once
#include "Vec3.hpp"

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
        float e[4];
    };

    Vec4() = default;
    Vec4(float _x, float _y, float _z, float _w);
    Vec4(const Vec3& vec3, float _w);

    Vec4& Homogenize();
    static Vec3 Homogenize(const Vec4& v);
    void Normalize();
    Vec4 operator+(const Vec4& other) const;
    Vec4 operator*(float other) const;

    float& operator[](int i);
    const float& operator[](int i) const;
};

inline Vec4::Vec4(float _x, float _y, float _z, float _w) : x{_x}, y{_y}, z{_z}, w{_w} {}
inline Vec4::Vec4(const Vec3& vec3, float _w = 1.0f)
{
    x = vec3.x;
    y = vec3.y;
    z = vec3.z;
    w = _w;
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
        return Vec3{v.x,v.y,v.z};
    }

    const Vec3 result{v.x / v.w, v.y / v.w, v.z / v.w};

    return result;
}

inline Vec4 Vec4::operator+(const Vec4& other) const { return {x + other.x, y + other.y, z + other.z, w + other.w}; }

inline Vec4 Vec4::operator*(const float other) const { return {x * other, y * other, z * other, w * other}; }

inline float& Vec4::operator[](const int i) { return e[i]; }

inline const float& Vec4::operator[](const int i) const { return e[i]; }
