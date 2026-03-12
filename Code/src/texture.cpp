#include "texture.hpp"

#include <sys/types.h>

#include "color.hpp"
#include "vec2.hpp"

#include <SDL2/SDL_image.h>


Texture::~Texture()
{
    if (m_texture != nullptr)
    {
        delete[] m_texture;
    }
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

inline Color* Texture::GetTexture() const
{
    return m_texture;
}

bool Texture::IsEmpty() const
{
    return m_texture == nullptr;
}

Color Texture::Get(float v, float u) const
{
    if (IsEmpty() || v > 1 || u > 1 || v < 0 || u < 0)
    {
        // std::cout << "Error: Trying to access invalid texture with parameters : u=>" << u << " v=>" << v <<
        // std::endl;
        return {255, 0, 255};
    }

    v *= (float) m_width;
    u *= (float) m_height;

    return m_texture[(int) v + ((int) u * m_width)];
}

bool Texture::Load_Image(const char* filename)
{
    SDL_Surface* surface = IMG_Load(filename);

    if (surface == nullptr)
    {
        return false;
    }

    SDL_Surface* rgba = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);

    m_texture = new Color[rgba->w * rgba->h];

    memcpy(m_texture, rgba->pixels, rgba->pitch * rgba->w * rgba->h);

    SDL_FreeSurface(surface);
    SDL_FreeSurface(rgba);

    return true;
}