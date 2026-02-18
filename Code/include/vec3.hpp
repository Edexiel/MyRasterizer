#pragma once

#include <cmath>

class Vec3
{
public:
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;

    Vec3() = default;
    Vec3(float x, float y, float z);

    static Vec3 Normalize(const Vec3& v);

    void Normalize();
    float GetMagnitude() const;

    Vec3 operator+(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;
    Vec3 operator*(float other) const;

    static Vec3 Zero() { return {0.0f, 0.0f, 0.0f}; }
    static Vec3 One() { return {1.0f, 1.0f, 1.0f}; }
    static Vec3 Up() { return {0.0f, 1.0f, 0.0f}; }
    static Vec3 Right() { return {1.0f, 0.0f, 0.0f}; }
    static Vec3 Left() { return {-1.0f, 0.0f, 0.0f}; }
    static Vec3 Forward() { return {0.0f, 0.0f, 1.0f}; }
    static Vec3 Backward() { return {0.0f, 0.0f, -1.0f}; }

    static float CrossProductZ(const Vec3& v1, const Vec3& v2);
    static Vec3 CrossProduct(const Vec3& v1, const Vec3& v2);
    static float DotProduct(const Vec3& v1, const Vec3& v2);
};

inline Vec3::Vec3(float x, float y, float z) : x{x}, y{y}, z{z}
{
}

inline Vec3 Vec3::Normalize(const Vec3& v)
{
    const float mag = v.GetMagnitude();
    if (mag == 0)
    {
        return Vec3{v};
    }

    return {v.x / mag, v.y / mag, v.z / mag};
}

inline void Vec3::Normalize()
{
    const float mag = GetMagnitude();

    if (mag == 0)
    {
        return;
    }

    x /= mag;
    y /= mag;
    z /= mag;
}

inline float Vec3::GetMagnitude() const
{
    return sqrtf((x * x) + (y * y) + (z * z));
}

inline Vec3 Vec3::operator+(const Vec3& other) const
{
    return {x + other.x, y + other.y, z + other.z};
}

inline Vec3 Vec3::operator-(const Vec3& other) const
{
    return {x - other.x, y - other.y, z - other.z};
}

inline Vec3 Vec3::operator*(float other) const
{
    return {x * other, y * other, z * other};
}

inline float Vec3::CrossProductZ(const Vec3& v1, const Vec3& v2)
{
    return (v1.x * v2.y) - (v2.x * v1.y);
}

inline Vec3 Vec3::CrossProduct(const Vec3& v1, const Vec3& v2)
{
    return {(v1.y * v2.z) - (v1.z * v2.y),
            (v1.z * v2.x) - (v1.x * v2.z),
            (v1.x * v2.y) - (v1.y * v2.x)};
}

inline float Vec3::DotProduct(const Vec3& v1, const Vec3& v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}
