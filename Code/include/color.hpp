#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <cmath>

template <typename T>
static constexpr T defaultAlpha = T{};

template <>
inline constexpr uint8_t defaultAlpha<uint8_t> = 255;

template <>
inline constexpr float defaultAlpha<float> = 1.f;

template <typename T>
class TColor
{
public:
    T r, g, b, a;

    constexpr TColor() noexcept : r{}, g{}, b{}, a{defaultAlpha<T>}
    {
    }

    constexpr TColor(T r, T g, T b, T a = defaultAlpha<T>) noexcept
        : r{r}, g{g}, b{b}, a{a}
    {
    }

    template <typename U>
    explicit constexpr TColor(const TColor<U>& other) noexcept
        : r{static_cast<T>(other.r)},
          g{static_cast<T>(other.g)},
          b{static_cast<T>(other.b)},
          a{static_cast<T>(other.a)}
    {
    }

    [[nodiscard]] constexpr TColor operator+(const TColor& rhs) const noexcept
    {
        return {static_cast<T>(r + rhs.r),
                static_cast<T>(g + rhs.g),
                static_cast<T>(b + rhs.b),
                static_cast<T>(a + rhs.a)};
    }

    TColor& operator+=(const TColor& rhs) noexcept
    {
        r = static_cast<T>(r + rhs.r);
        g = static_cast<T>(g + rhs.g);
        b = static_cast<T>(b + rhs.b);
        a = static_cast<T>(a + rhs.a);
        return *this;
    }

    [[nodiscard]] constexpr TColor operator*(const TColor& rhs) const noexcept
    {
        return {static_cast<T>(r * rhs.r),
                static_cast<T>(g * rhs.g),
                static_cast<T>(b * rhs.b),
                static_cast<T>(a * rhs.a)};
    }

    TColor& operator*=(const TColor& rhs) noexcept
    {
        r = static_cast<T>(r * rhs.r);
        g = static_cast<T>(g * rhs.g);
        b = static_cast<T>(b * rhs.b);
        a = static_cast<T>(a * rhs.a);
        return *this;
    }

    template <typename U>
    [[nodiscard]] constexpr TColor operator*(U scalar) const noexcept
    {
        return {static_cast<T>(r * scalar),
                static_cast<T>(g * scalar),
                static_cast<T>(b * scalar),
                static_cast<T>(a * scalar)};
    }

    TColor& operator*=(T scalar) noexcept
    {
        r = static_cast<T>(r * scalar);
        g = static_cast<T>(g * scalar);
        b = static_cast<T>(b * scalar);
        a = static_cast<T>(a * scalar);
        return *this;
    }

    [[nodiscard]] constexpr bool operator==(const TColor& rhs) const noexcept
    {
        return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
    }

    [[nodiscard]] constexpr bool operator!=(const TColor& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    constexpr TColor Clamped(T lo, T hi) const noexcept
    {
        return
        {
            std::clamp<T>(r, lo, hi),
            std::clamp<T>(g, lo, hi),
            std::clamp<T>(b, lo, hi),
            std::clamp<T>(a, lo, hi)
        };
    }

    // Rec 709 perceived luminance
    [[nodiscard]] constexpr double Luminance() const noexcept
    {
        return 0.2126 * static_cast<double>(r) +
               0.7152 * static_cast<double>(g) +
               0.0722 * static_cast<double>(b);
    }

    template <typename U>
    [[nodiscard]] constexpr TColor Lerp(const TColor& other, U t) const noexcept
    {
        return {std::lerp<U>(r, other.r,t), std::lerp<U>(g, other.g,t), std::lerp<U>(b, other.b,t), std::lerp<U>(a, other.a,t)};
    }

    friend std::ostream& operator<<(std::ostream& os, const TColor& c)
    {
        os << "Color(" << +c.r << ", " << +c.g << ", " << +c.b << ", " << +c.a << ")";
        return os;
    }
};

template <typename T>
constexpr TColor<T> operator*(T scalar, const TColor<T>& c) noexcept
{
    return c * scalar;
}

using ColorF = TColor<float>;
using ColorD = TColor<double>;
using Color16 = TColor<uint16_t>;
using Color = TColor<uint8_t>;

namespace Colors
{
// primaries & secondaries
static constexpr Color Black{0, 0, 0, 255};
static constexpr Color White{255, 255, 255, 255};
static constexpr Color Red{255, 0, 0, 255};
static constexpr Color Green{0, 255, 0, 255};
static constexpr Color Blue{0, 0, 255, 255};
static constexpr Color Yellow{255, 255, 0, 255};
static constexpr Color Cyan{0, 255, 255, 255};
static constexpr Color Magenta{255, 0, 255, 255};

// greys
static constexpr Color DarkGrey{64, 64, 64, 255};
static constexpr Color Grey{128, 128, 128, 255};
static constexpr Color LightGrey{192, 192, 192, 255};
static constexpr Color Silver{192, 192, 192, 255};

// warm
static constexpr Color Orange{255, 165, 0, 255};
static constexpr Color Gold{255, 215, 0, 255};
static constexpr Color Pink{255, 192, 203, 255};
static constexpr Color HotPink{255, 105, 180, 255};
static constexpr Color Coral{255, 127, 80, 255};
static constexpr Color Salmon{250, 128, 114, 255};
static constexpr Color Crimson{220, 20, 60, 255};
static constexpr Color Maroon{128, 0, 0, 255};
static constexpr Color Brown{165, 42, 42, 255};

// cool
static constexpr Color Navy{0, 0, 128, 255};
static constexpr Color RoyalBlue{65, 105, 225, 255};
static constexpr Color SkyBlue{135, 206, 235, 255};
static constexpr Color CornflowerBlue{100, 149, 237, 255};
static constexpr Color Teal{0, 128, 128, 255};
static constexpr Color Turquoise{64, 224, 208, 255};
static constexpr Color SteelBlue{70, 130, 180, 255};
static constexpr Color SlateBlue{106, 90, 205, 255};

// greens
static constexpr Color Lime{191, 255, 0, 255};
static constexpr Color ForestGreen{34, 139, 34, 255};
static constexpr Color OliveGreen{128, 128, 0, 255};
static constexpr Color Mint{152, 255, 152, 255};

// purples
static constexpr Color Purple{128, 0, 128, 255};
static constexpr Color Violet{238, 130, 238, 255};
static constexpr Color Indigo{75, 0, 130, 255};
static constexpr Color Lavender{230, 230, 250, 255};

// utility
static constexpr Color Transparent{0, 0, 0, 0};
}