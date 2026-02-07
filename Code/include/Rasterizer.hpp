#pragma once

#include <SDL_surface.h>
#include "Light.hpp"
#include "Scene.hpp"
#include "Texture.hpp"
#include "Vertex.hpp"

class Rasterizer
{
private:
    uint width;
    uint height;

    // Color* color_buffer;
    float* depth_buffer;

    SDL_Surface* colorBuffer = nullptr;


    void DrawTriangle(const Vertex(&vertices)[3], const Mat4& transformation, const Light& light, const Vec2f* UV = nullptr, const Texture* texture = nullptr) const;

    void RasterTriangle(const Vertex(&vertices)[3], const Vec4(&t_vertices)[3], const Vec4(&p_vertices)[3], const Vec4(&t_normals)[3], const Light& light, const Vec2f* UV = nullptr, const Texture* texture = nullptr) const;

    void DrawLine(const Vertex(&vertices)[2], const Mat4& model);

    void RasterLine(const Vertex* vertex);

    // void DrawPoint(Vertex v1, Mat4& transfo);

    void SetPixelColor(uint x, uint y, float z, const Color& c) const;

public:
    Rasterizer(uint width, uint height);
    ~Rasterizer();

    SDL_Surface* GetColorBuffer();

    Mat4 projection;
    Mat4 viewport;
    Mat4 view;

    void RenderScene(Scene& scene);

    void ClearColorBuffer() const;
    void ClearDepthBuffer() const;
};
