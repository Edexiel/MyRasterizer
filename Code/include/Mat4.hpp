#pragma once
#include <cmath>
#include "Vec4.hpp"

// Column major library
class Mat4
{
public:
    union
    {
        Vec4 v[4]{};
        float a[16];
        float aa[4][4];
    };

    Mat4();
    explicit Mat4(float f);
    explicit Mat4(const float (&array)[16]);
    Mat4(const Vec4& v1, const Vec4& v2, const Vec4& v3, const Vec4& v4);

    static Mat4 Identity();
    static Mat4 ViewportMatrix(float x, float y, float width, float height);
    static Mat4 OrthoMatrix(float left, float right, float bottom, float top, float near, float far);
    static Mat4 Perspective(float fovy, float aspectRatio, float front, float back);
    static Mat4 CreateTranslationMatrix(const Vec3& translation);
    static Mat4 CreateScaleMatrix(const Vec3& scale);
    static Mat4 CreateRotationMatrix(const Vec3& rotation);
    static Mat4 CreateXRotationMatrix(float angle);
    static Mat4 CreateYRotationMatrix(float angle);
    static Mat4 CreateZRotationMatrix(float angle);
    static Mat4 CreateTransformMatrix(const Vec3& position, const Vec3& rotation, const Vec3& scale);
    static Mat4 Zero();
    Vec3 TransformPoint(const Vec3& vector) const;   // w = 1
    Vec3 TransformVector(const Vec3& vector) const; // w = 0`
    Mat4 operator+(const Mat4& other) const;
    Mat4 operator*(const Mat4& other) const;
    Vec4 operator*(const Vec4& vector) const;
    Vec4& operator[](int i);
};

inline Mat4::Mat4() : Mat4(0)
{
}

inline Mat4::Mat4(float f)
{
    for (float& i : a)
    {
        i = f;
    }
}

inline Mat4::Mat4(const Vec4& v1, const Vec4& v2, const Vec4& v3, const Vec4& v4)
{
    v[0] = v1;
    v[1] = v2;
    v[2] = v3;
    v[3] = v4;
}

inline Mat4 Mat4::Identity()
{
    Mat4 identity{0};
    identity.a[0] = 1;
    identity.a[5] = 1;
    identity.a[10] = 1;
    identity.a[15] = 1;
    return identity;
}

inline Mat4 Mat4::ViewportMatrix(float x, float y, float width, float height)
{
    Mat4 viewport{};
    viewport.a[0] = width / 2;
    viewport.a[5] = height / 2;
    viewport.a[10] = 0.5f;
    viewport.a[12] = x + width / 2;
    viewport.a[13] = y + height / 2;
    viewport.a[14] = 0.5f;
    viewport.a[15] = 0;

    return viewport;
}

inline Mat4 Mat4::OrthoMatrix(float left, float right, float bottom, float top, float near, float far)
{
    Mat4 result{};
    result.a[0] = 2.f / (right - left);
    result.a[3] = -((right + left) / (right - left));
    result.a[5] = 2.f / (top - bottom);
    result.a[7] = -((top + bottom) / (top - bottom));
    result.a[10] = -2.f / (far - near);
    result.a[11] = -((far + near) / (far - near));
    result.a[15] = 1.f;

    return result;
}

inline Mat4 Mat4::Perspective(float fovy, float aspectRatio, float front, float back)
{
    const float tangent = tanf(fovy * M_PIf / 360.0f); // tangent of half fovY
    const float top = front * tangent; // half height of near plane
    const float right = top * aspectRatio; // half width of near plane

    Mat4 matrix{};
    matrix.a[0] = front / right;
    matrix.a[5] = front / top;
    matrix.a[10] = -(back + front) / (back - front);
    matrix.a[11] = -1;
    matrix.a[14] = -(2 * back * front) / (back - front);
    return matrix;
}

inline Mat4 Mat4::CreateTranslationMatrix(const Vec3& t)
{
    return Mat4
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {t.x, t.y, t.z, 1}
    };
}

inline Mat4 Mat4::CreateScaleMatrix(const Vec3& scale)
{
    return Mat4
    {
        {scale.x, 0, 0, 0},
        {0, scale.y, 0, 0},
        {0, 0, scale.z, 0},
        {0, 0, 0, 1}
    };
}

inline Mat4 Mat4::CreateRotationMatrix(const Vec3& rotation)
{
    return CreateXRotationMatrix(rotation.x) * CreateYRotationMatrix(rotation.y) * CreateZRotationMatrix(rotation.z);
}

inline Mat4 Mat4::CreateXRotationMatrix(const float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    return Mat4
    {
        {1, 0, 0, 0},
        {0, c, s, 0},
        {0, -s, c, 0},
        {0, 0, 0, 1}
    };
}

inline Mat4 Mat4::CreateYRotationMatrix(const float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    return Mat4{
        {c, 0, -s, 0},
        {0, 1, 0, 0},
        {s, 0, c, 0},
        {0, 0, 0, 1}
    };
}

inline Mat4 Mat4::CreateZRotationMatrix(const float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    return Mat4
    {
        {c, s, 0, 0},
        {-s, c, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
}

inline Mat4 Mat4::CreateTransformMatrix(const Vec3& position, const Vec3& rotation, const Vec3& scale)
{
    return CreateTranslationMatrix(position) * CreateRotationMatrix(rotation) * CreateScaleMatrix(scale);
}

inline Vec3 Mat4::TransformPoint(const Vec3& vector) const
{
    const float x = vector.x * v[0].e[0] + vector.y * v[1].e[0] + vector.z * v[2].e[0] + v[3].e[0];
    const float y = vector.x * v[0].e[1] + vector.y * v[1].e[1] + vector.z * v[2].e[1] + v[3].e[1];
    const float z = vector.x * v[0].e[2] + vector.y * v[1].e[2] + vector.z * v[2].e[2] + v[3].e[2];
    const float w = vector.x * v[0].e[3] + vector.y * v[1].e[3] + vector.z * v[2].e[3] + v[3].e[3];

    if (w != 0.0f)
    {
        return Vec3{x / w, y / w, z / w};
    }

    return Vec3{x, y, z};
}

inline Vec3 Mat4::TransformVector(const Vec3& vector) const
{
    return Vec3{
        vector.x * v[0].e[0] + vector.y * v[1].e[0] + vector.z * v[2].e[0],
        vector.x * v[0].e[1] + vector.y * v[1].e[1] + vector.z * v[2].e[1],
        vector.x * v[0].e[2] + vector.y * v[1].e[2] + vector.z * v[2].e[2],
    };
}

inline Mat4 Mat4::operator+(const Mat4& other) const
{
    Mat4 res{0};

    for (int i = 0; i < 16; i++)
        res.a[i] = a[i] + other.a[i];

    return res;
}

inline Mat4 Mat4::operator*(const Mat4& other) const
{
    Mat4 res = Mat4{0};

    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r)
            for (int i = 0; i < 4; ++i)
                res.v[c].e[r] += v[i].e[r] * other.v[c].e[i];
    return res;
}

inline Vec4 Mat4::operator*(const Vec4& vector) const
{
    Vec4 res{0, 0, 0, 0};

    // for (int i = 0; i < 4; i++)
    // {
    //     res.e[i] = 0;
    //     for (int j = 0; j < 4; j++)
    //         res.e[i] += v[j].e[i] * _v.e[j];
    // }

    res.x = vector.x * v[0].e[0] + vector.y * v[1].e[0] + vector.z * v[2].e[0] + vector.w * v[3].e[0];
    res.y = vector.x * v[0].e[1] + vector.y * v[1].e[1] + vector.z * v[2].e[1] + vector.w * v[3].e[1];
    res.z = vector.x * v[0].e[2] + vector.y * v[1].e[2] + vector.z * v[2].e[2] + vector.w * v[3].e[2];
    res.w = vector.x * v[0].e[3] + vector.y * v[1].e[3] + vector.z * v[2].e[3] + vector.w * v[3].e[3];

    return res;
}

inline Vec4& Mat4::operator[](const int i) { return v[i]; }
