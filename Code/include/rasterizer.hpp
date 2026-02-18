#pragma once

#include <SDL_surface.h>

#include "color.hpp"
#include "light.hpp"
#include "mat4.hpp"
#include "scene.hpp"
#include "texture.hpp"
#include "vec2.hpp"
#include "vec4.hpp"
#include "vertex.hpp"

class Rasterizer
{
public:
    Rasterizer(uint width, uint height);
    ~Rasterizer();
    SDL_Surface* GetColorBuffer() const;


    Mat4 projection;
    Mat4 viewport;
    Mat4 view;

    void RenderScene(Scene& scene);

    void ClearColorBuffer() const;
    void ClearDepthBuffer() const;

private:
    uint m_width{};
    uint m_height{};

    float* m_depth_buffer;

    SDL_Surface* m_color_buffer = nullptr;

    void DrawTriangle(const std::array<Vertex,3>& vertices,
                             const Mat4& transformation,
                             const Light& light,
                             const std::array<Vec2f, 3>* uv = nullptr,
                             const Texture* texture = nullptr);

    void RasterTriangle(const std::array<Vertex,3> &vertices,
                               const std::array<Vec4,3>&tVertices,
                               const std::array<Vec4,3>&pVertices,
                               const std::array<Vec4,3>&tNormals,
                               const Light& light,
                               const std::array<Vec2f, 3>* uv = nullptr,
                               const Texture* texture = nullptr);

    void DrawLine(const std::array<Vertex,2>& vertices, const Mat4& transformation);

    void RasterLine(const Vertex* vertex);

    // void DrawPoint(Vertex v1, Mat4& transfo);

    void SetPixelColor(uint x, uint y, float z, const Color& c) const;
};
