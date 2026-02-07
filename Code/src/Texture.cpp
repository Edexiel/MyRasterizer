#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

Texture::Texture() : _width{0}, _height{0}, _texture{nullptr} {}

Texture::~Texture() { delete[] _texture; }

Vec2<uint> Texture::GetSize() { return {_width, _height}; }

uint Texture::GetWidth() const { return _width; }

uint Texture::GetHeight() const { return _height; }

bool Texture::Load_PNG(const char* filename)
{
    // stbi_set_flip_vertically_on_load(true);
    // int channels;
    // _texture = (Color*)stbi_load(filename, &_width, &_height, &channels, 3);

    // texture = new Color[width * height];

    // for (int i = 0; i < width * height; i++)
    //     texture[i] = _texture[i];

    // memcpy(texture, _texture, width * height * sizeof(Color));

    // stbi_image_free(_texture);
    return _texture != nullptr;
}

inline Color* Texture::GetTexture() const { return _texture; }

bool Texture::IsEmpty() const { return _texture == nullptr; }

Color Texture::Accessor(float v, float u) const
{
    if (IsEmpty() || v > 1 || u > 1 || v < 0 || u < 0)
    {
        // std::cout << "Error: Trying to access invalid texture with parameters : u=>" << u << " v=>" << v <<
        // std::endl;
        return {255, 0, 0};
    }

    // wtf ?
    v *= _width;
    u *= _height;

    return this->_texture[(int)v + (int)u * _width];
}
