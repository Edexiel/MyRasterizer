#pragma once

using uint = unsigned int;

template <typename T>
inline T max(T a, T b)
{
    return a > b ? a : b;
}

template <typename T>
inline T min(T a, T b)
{
    return a < b ? a : b;
}

enum E_DRAW_MODE
{
    point,
    triangle,
    line,
    form
};

template <class T>
inline T clamp(T v, T lo, T hi)
{
    // assert( !(hi < lo) );
    return (v < lo) ? lo : (hi < v) ? hi : v;
}
