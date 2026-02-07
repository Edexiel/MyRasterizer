#pragma once

#include "Color.hpp"
#include "Vec2.hpp"

class Texture
{
private:
    uint _width;
    uint _height;

    Color* _texture;

public:
    Texture();
    ~Texture();

    Vec2<uint> GetSize();

    uint GetWidth() const;
    uint GetHeight() const;

    Color* GetTexture() const;

    bool IsEmpty() const;

    Color Accessor(float v, float u) const;

    bool Load_PNG(const char* filename);
};
