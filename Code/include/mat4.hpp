#pragma once
#include <cmath>

#include "vec3.hpp"
#include "vec4.hpp"

// Column major library
class Mat4
{
public:
    union
    {
        Vec4 vectors[4]{};
        float values[16];
        float array[4][4];
    };

    Mat4();
    explicit Mat4(float f);
    explicit Mat4(const float (&array)[16]);
    Mat4(const Vec4& v1, const Vec4& v2, const Vec4& v3, const Vec4& v4);

    static Mat4 Identity();
    static Mat4 ViewportMatrix(float x, float y, float width, float height);
    static Mat4 OrthoMatrix(float left, float right, float bottom, float top, float near, float far);
    static Mat4 Perspective(float fovy, float aspectRatio, float near, float far);
    static Mat4 CreateTranslationMatrix(const Vec3& translation);
    static Mat4 CreateScaleMatrix(const Vec3& scale);
    static Mat4 CreateRotationMatrix(const Vec3& rotation);
    static Mat4 CreateXRotationMatrix(float angle);
    static Mat4 CreateYRotationMatrix(float angle);
    static Mat4 CreateZRotationMatrix(float angle);
    static Mat4 CreateTransformMatrix(const Vec3& position, const Vec3& rotation, const Vec3& scale);
    static Mat4 Transpose(const Mat4& matrix);
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
    for (float& i : values)
    {
        i = f;
    }
}

inline Mat4::Mat4(const Vec4& v1, const Vec4& v2, const Vec4& v3, const Vec4& v4)
{
    vectors[0] = v1;
    vectors[1] = v2;
    vectors[2] = v3;
    vectors[3] = v4;
}

inline Mat4 Mat4::Identity()
{
    Mat4 identity{0};
    identity.values[0] = 1;
    identity.values[5] = 1;
    identity.values[10] = 1;
    identity.values[15] = 1;
    return identity;
}

inline Mat4 Mat4::ViewportMatrix(float x, float y, float width, float height)
{
    Mat4 viewport{};
    viewport.values[0] = width / 2;
    viewport.values[5] = -height / 2;
    viewport.values[10] = 1.f;
    viewport.values[12] = x + (width / 2);
    viewport.values[13] = y + (height / 2);
    viewport.values[14] = 0.f;
    viewport.values[15] = 1;

    return viewport;
}

inline Mat4 Mat4::OrthoMatrix(float left, float right, float bottom, float top, float near, float far)
{
    Mat4 m{};

    const float rl = right - left;
    const float tb = top - bottom;
    const float fn = far - near;

    // Scale
    m.values[0]  = 2.0f / rl;
    m.values[5]  = 2.0f / tb;
    m.values[10] = 1.0f / fn;

    // Translation
    m.values[12] = -(right + left) / rl;
    m.values[13] = -(top + bottom) / tb;
    m.values[14] = -near / fn;
    m.values[15] = 1.0f;

    return m;
}
inline Mat4 Mat4::Perspective(float fovy, float aspectRatio, float near, float far)
{
    const float f = 1.0f / tanf(fovy * M_PIf / 360.0f);

    Mat4 matrix{};
    matrix.values[0] = f / aspectRatio;
    matrix.values[5] = f;
    matrix.values[10] = far / (far - near);
    matrix.values[11] = 1;
    matrix.values[14] = -(near * far) / (far - near);

    return matrix;
}

inline Mat4 Mat4::CreateTranslationMatrix(const Vec3& t)
{
    return Mat4{{1, 0, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 1, 0},
                {t.x, t.y, t.z, 1}};
}

inline Mat4 Mat4::CreateScaleMatrix(const Vec3& scale)
{
    return Mat4{{scale.x, 0, 0, 0},
                {0, scale.y, 0, 0},
                {0, 0, scale.z, 0},
                {0, 0, 0, 1}};
}

inline Mat4 Mat4::CreateRotationMatrix(const Vec3& rotation)
{
    return CreateXRotationMatrix(rotation.x) * CreateYRotationMatrix(rotation.y) * CreateZRotationMatrix(rotation.z);
}

inline Mat4 Mat4::CreateXRotationMatrix(const float angle)
{
    float const c = cosf(angle);
    float const s = sinf(angle);

    return Mat4{{1, 0, 0, 0},
                {0, c, -s, 0},
                {0, s, c, 0},
                {0, 0, 0, 1}};
}

inline Mat4 Mat4::CreateYRotationMatrix(const float angle)
{
    float const c = cosf(angle);
    float const s = sinf(angle);

    return Mat4{{c, 0, s, 0},
                {0, 1, 0, 0},
                {-s, 0, c, 0},
                {0, 0, 0, 1}};
}

inline Mat4 Mat4::CreateZRotationMatrix(const float angle)
{
    float const c = cosf(angle);
    float const s = sinf(angle);

    return Mat4{{c, -s, 0, 0},
                {s, c, 0, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 1}};
}

inline Mat4 Mat4::CreateTransformMatrix(const Vec3& position, const Vec3& rotation, const Vec3& scale)
{
    return CreateTranslationMatrix(position) * CreateRotationMatrix(rotation) * CreateScaleMatrix(scale);
}
inline Mat4 Mat4::Transpose(const Mat4& matrix)
{
    Mat4 transpose{};
        for(int n = 0; n<16; n++)
        {
            int i = n / 4;
            int j = n % 4;
            transpose.values[n] = matrix.values[4*j + i];
        }
    return transpose;
}

inline Vec3 Mat4::TransformPoint(const Vec3& vector) const
{
    const float x = (vector.x * vectors[0].array[0]) + (vector.y * vectors[1].array[0]) + (vector.z * vectors[2].array[0]) + vectors[3].array[0];
    const float y = (vector.x * vectors[0].array[1]) + (vector.y * vectors[1].array[1]) + (vector.z * vectors[2].array[1]) + vectors[3].array[1];
    const float z = (vector.x * vectors[0].array[2]) + (vector.y * vectors[1].array[2]) + (vector.z * vectors[2].array[2]) + vectors[3].array[2];
    const float w = (vector.x * vectors[0].array[3]) + (vector.y * vectors[1].array[3]) + (vector.z * vectors[2].array[3]) + vectors[3].array[3];

    if (w != 0.0f)
    {
        return Vec3{x / w, y / w, z / w};
    }

    return Vec3{x, y, z};
}

inline Vec3 Mat4::TransformVector(const Vec3& vector) const
{
    return Vec3{
        (vector.x * vectors[0].array[0]) + (vector.y * vectors[1].array[0]) + (vector.z * vectors[2].array[0]),
        (vector.x * vectors[0].array[1]) + (vector.y * vectors[1].array[1]) + (vector.z * vectors[2].array[1]),
        (vector.x * vectors[0].array[2]) + (vector.y * vectors[1].array[2]) + (vector.z * vectors[2].array[2]),
    };
}

inline Mat4 Mat4::operator+(const Mat4& other) const
{
    Mat4 res{0};

    for (int i = 0; i < 16; i++)
    {
        res.values[i] = values[i] + other.values[i];
    }

    return res;
}

inline Mat4 Mat4::operator*(const Mat4& other) const
{
    Mat4 res = Mat4{0};

    for (int c = 0; c < 4; ++c)
    {
        for (int r = 0; r < 4; ++r)
        {
            for (int i = 0; i < 4; ++i)
            {
                res.vectors[c].array[r] += vectors[i].array[r] * other.vectors[c].array[i];
            }
        }
    }
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

    res.x = (vector.x * vectors[0].array[0]) + (vector.y * vectors[1].array[0]) + (vector.z * vectors[2].array[0]) +
              (vector.w * vectors[3].array[0]);
    res.y = (vector.x * vectors[0].array[1]) + (vector.y * vectors[1].array[1]) + (vector.z * vectors[2].array[1]) +
              (vector.w * vectors[3].array[1]);
    res.z = (vector.x * vectors[0].array[2]) + (vector.y * vectors[1].array[2]) + (vector.z * vectors[2].array[2]) +
              (vector.w * vectors[3].array[2]);
    res.w = (vector.x * vectors[0].array[3]) + (vector.y * vectors[1].array[3]) + (vector.z * vectors[2].array[3]) +
              (vector.w * vectors[3].array[3]);

    return res;
}

inline Vec4& Mat4::operator[](const int i)
{
    return vectors[i];
}
