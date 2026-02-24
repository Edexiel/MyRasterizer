#pragma once

#include <sys/types.h>

#include "color.hpp"
#include "vec2.hpp"

class Texture
{
public:
    Texture() = default;
    ~Texture();

    Vec2<uint> GetSize() const;

    uint GetWidth() const;
    uint GetHeight() const;

    Color* GetTexture() const;

    bool IsEmpty() const;

    Color Accessor(float v, float u) const;

    Color operator [] (int index) const {return m_texture[index];}
    Color& operator [] (int index) {return m_texture[index];}

    bool Load_PNG(const char* filename);

private:
    uint m_width{0};
    uint m_height{0};

    Color* m_texture = nullptr;
};
