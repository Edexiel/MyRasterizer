#pragma once

template <typename T>
class Vec2
{
public:
    union
    {
        struct
        {
            T x;
            T y;
        };

        T array[2];
    };

    Vec2() = default;
    Vec2(T /*_x*/, T /*_y*/);

    T GetMagnitude() const;
    void Normalize();
    Vec2<T> operator+(Vec2<T>& other);
    Vec2<T> operator*(T& other);
};

#define Vec2f Vec2<float>
#define Vec2d Vec2<double>
#define Vec2b Vec2<bool>

template <typename T>
inline Vec2<T>::Vec2(T x, T y) : x{x}, y{y}
{
}

template <typename T>
inline T Vec2<T>::GetMagnitude() const
{
    return sqrtf(x * x * y * y);
}

template <typename T>
inline void Vec2<T>::Normalize()
{
    float mag = GetMagnitude();

    if (mag == 0)
    {
        return;
    }

    x /= mag;
    y /= mag;
}

template <typename T>
Vec2<T> Vec2<T>::operator+(Vec2<T>& other)
{
    return {x + other.x, y + other.y};
}

template <typename T>
Vec2<T> Vec2<T>::operator*(T& other)
{
    return {x * other, y * other};
}
