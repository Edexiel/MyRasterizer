#include "texture.hpp"

#include <sys/types.h>

#include "color.hpp"
#include "vec2.hpp"

#define STB_IMAGE_IMPLEMENTATION


Texture::~Texture()
{
    delete[] m_texture;
}

Vec2<uint> Texture::GetSize() const
{
    return {m_width, m_height};
}

uint Texture::GetWidth() const
{
    return m_width;
}

uint Texture::GetHeight() const
{
    return m_height;
}

bool Texture::Load_PNG(const char* /*filename*/)
{
    // stbi_set_flip_vertically_on_load(true);
    // int channels;
    // _texture = (Color*)stbi_load(filename, &_width, &_height, &channels, 3);

    // texture = new Color[width * height];

    // for (int i = 0; i < width * height; i++)
    //     texture[i] = _texture[i];

    // memcpy(texture, _texture, width * height * sizeof(Color));

    // stbi_image_free(_texture);
    return m_texture != nullptr;
}

inline Color* Texture::GetTexture() const
{
    return m_texture;
}

bool Texture::IsEmpty() const
{
    return m_texture == nullptr;
}

Color Texture::Accessor(float v, float u) const
{
    if (IsEmpty() || v > 1 || u > 1 || v < 0 || u < 0)
    {
        // std::cout << "Error: Trying to access invalid texture with parameters : u=>" << u << " v=>" << v <<
        // std::endl;
        return {255, 0, 0};
    }

    // wtf ?
    v *= m_width;
    u *= m_height;

    return m_texture[(int) v + ((int) u * m_width)];
}
