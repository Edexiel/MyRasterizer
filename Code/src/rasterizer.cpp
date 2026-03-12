#include "rasterizer.hpp"

#include <SDL_blendmode.h>
#include <SDL_pixels.h>
#include <SDL_surface.h>
#include <algorithm>
#include <array>
#include <cfloat>
#include <cmath>
#include <memory>
#include <utility>

#include "color.hpp"
#include "entity.hpp"
#include "light.hpp"
#include "mat4.hpp"
#include "mesh.hpp"
#include "scene.hpp"
#include "texture.hpp"
#include "tools.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "vertex.hpp"

Rasterizer::Rasterizer(int width, int height)
    : m_width{width}, m_height{height}
{
    m_depth_buffer = new float[width * height];
    m_color_buffer = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_SetSurfaceBlendMode(m_color_buffer, SDL_BLENDMODE_NONE);

    // Initialize buffers
    ClearColorBuffer();
    ClearDepthBuffer();

    const float aspect = static_cast<float>(width) / static_cast<float>(height);

    viewport = Mat4::ViewportMatrix(0.f, 0.f, (float) width, (float) height);
#if 1 // Perspective or 2D
    projection = Mat4::Perspective(60.f, aspect, 0.01f, 50.f);
#else
    projection = Mat4::OrthoMatrix(-aspect, aspect, -1.f, 1.f, 0.f, 100.f);
#endif

    view = Mat4::Identity();
}

Rasterizer::~Rasterizer()
{
    SDL_FreeSurface(m_color_buffer);
    delete[] m_depth_buffer;
}

SDL_Surface* Rasterizer::GetColorBuffer() const
{
    return m_color_buffer;
}

void Rasterizer::RenderScene(Scene& scene)
{
    ClearColorBuffer();
    ClearDepthBuffer();

    for (const Entity& entity : scene.GetEntities())
    {
        switch (entity.GetDrawMode())
        {
            case triangle: {
                if (entity.GetMesh()->indices.size() < 3)
                {
                    return;
                }

                for (uint i = 0; i < entity.GetMesh()->indices.size() - 2; i += 3)
                {
                    const std::shared_ptr<Mesh>& mesh = entity.GetMesh();
                    const std::array<Vertex, 3> triangle{mesh->vertices[mesh->indices[i]],
                                                         mesh->vertices[mesh->indices[i + 1]],
                                                         mesh->vertices[mesh->indices[i + 2]]};

                    if (entity.GetMesh()->uv.empty() || entity.GetMesh()->texture.IsEmpty())
                    {
                        DrawTriangle(triangle, entity.GetTransform(), scene.GetLights()[0]);
                    }
                    else
                    {
                        const std::array<Vec2f, 3> uv{entity.GetMesh()->uv[i],
                                                      entity.GetMesh()->uv[i + 1],
                                                      entity.GetMesh()->uv[i + 2]};

                        DrawTriangle(triangle, entity.GetTransform(), scene.GetLights()[0], &uv, &entity.GetMesh()->texture);
                    }
                }

                break;
            }
            case line: {
                if (entity.GetMesh()->indices.size() < 2)
                {
                    break;
                }

                for (uint i = 0; i < entity.GetMesh()->indices.size() - 1; i += 1)
                {
                    const std::array<Vertex, 2> line{entity.GetMesh()->vertices[entity.GetMesh()->indices[i]],
                                                     entity.GetMesh()->vertices[entity.GetMesh()->indices[i + 1]]};

                    DrawLine(line, entity.GetTransform());
                }
                break;
            }
            default:
                break;
        }
    }
}

inline void Rasterizer::DrawTriangle(const std::array<Vertex, 3>& vertices,
                                     const Mat4& transformation,
                                     const Light& light,
                                     const std::array<Vec2f, 3>* uv,
                                     const Texture* texture)
{
    // transform space: transformation * vec3      (4D)
    // clipSpace:            transformation * vec3 (4D) [-w,w]
    //      clipping out of bound triangles (0001 0010 0100)
    // NDC:  vec3/vec4.w                          (3D) [-1,1]
    //      Back face culling
    // Screen coordinate : viewport * ndc        (2D)

    std::array<Vec4, 3> transformCoordinates;
    std::array<Vec4, 3> transformNormals;

    for (int i = 0; i < 3; i++)
    {
        transformCoordinates[i] = view * transformation * Vec4{vertices[i].position, 1.f};
        transformNormals[i] = transformation * Vec4{vertices[i].normal, 0.f};
    }

    std::array<Vec4, 3> clipCoord;
    for (int i = 0; i < 3; i++)
    {
        clipCoord[i] = projection * transformCoordinates[i];
    }

    // Clipping — cull only if all 3 vertices are outside the SAME frustum plane
    if ((clipCoord[0].x < -clipCoord[0].w && clipCoord[1].x < -clipCoord[1].w && clipCoord[2].x < -clipCoord[2].w) || // left
        (clipCoord[0].x > clipCoord[0].w && clipCoord[1].x > clipCoord[1].w && clipCoord[2].x > clipCoord[2].w) ||    // right
        (clipCoord[0].y < -clipCoord[0].w && clipCoord[1].y < -clipCoord[1].w && clipCoord[2].y < -clipCoord[2].w) || // bottom
        (clipCoord[0].y > clipCoord[0].w && clipCoord[1].y > clipCoord[1].w && clipCoord[2].y > clipCoord[2].w) ||    // top
        (clipCoord[0].z < -clipCoord[0].w && clipCoord[1].z < -clipCoord[1].w && clipCoord[2].z < -clipCoord[2].w) || // near
        (clipCoord[0].z > clipCoord[0].w && clipCoord[1].z > clipCoord[1].w && clipCoord[2].z > clipCoord[2].w))      // far
    {
        return;
    }

    std::array<Vec3, 3> ndc;
    for (int i = 0; i < 3; i++)
    {
        ndc[i] = Vec4::Homogenize(clipCoord[i]);
    }

    // back face culling
    if (Vec3::CrossProductZ(ndc[2] - ndc[0], ndc[1] - ndc[0]) <= 0.f)
    {
        return;
    }

    std::array<Vertex, 3> screenCoord;
    for (int i = 0; i < 3; i++)
    {
        screenCoord[i] = Vertex{viewport.TransformPoint(ndc[i]), vertices[i].color, vertices[i].normal};
    }

    // Light const correctedLight{light};
    // corrected_light.Correct(view);
    // corrected_light.SetPosition(Vec3{0.f, 0.f, 0.f});

    RasterTriangle(screenCoord, transformCoordinates, clipCoord, transformNormals, light, uv, texture);
}


inline void Rasterizer::RasterTriangle(const std::array<Vertex, 3>& vertices,
                                       const std::array<Vec4, 3>& tVertices,
                                       const std::array<Vec4, 3>& pVertices,
                                       const std::array<Vec4, 3>& tNormals,
                                       const Light& light,
                                       const std::array<Vec2f, 3>* uv,
                                       const Texture* texture)
{
    // shortcuts
    const Vertex& v0 = vertices[0];
    const Vertex& v1 = vertices[1];
    const Vertex& v2 = vertices[2];

    // Homogenized vertices
    const Vec3 hv0 = Vec4::Homogenize(tVertices[0]);
    const Vec3 hv1 = Vec4::Homogenize(tVertices[1]);
    const Vec3 hv2 = Vec4::Homogenize(tVertices[2]);

    // Homogenized normals
    const Vec3 hn0 = Vec4::Homogenize(tNormals[0]);
    const Vec3 hn1 = Vec4::Homogenize(tNormals[1]);
    const Vec3 hn2 = Vec4::Homogenize(tNormals[2]);

    const float invW0 = 1.f / pVertices[0].w;
    const float invW1 = 1.f / pVertices[1].w;
    const float invW2 = 1.f / pVertices[2].w;

    // Bounding box
    const int xMin = (int) max(min(min(v0.position.x, v1.position.x), v2.position.x), 0.f);
    const int yMin = (int) max(min(min(v0.position.y, v1.position.y), v2.position.y), 0.f);
    const int xMax = (int) min(max(max(v0.position.x, v1.position.x), v2.position.x), (float) m_width - 1);
    const int yMax = (int) min(max(max(v0.position.y, v1.position.y), v2.position.y), (float) m_height - 1);

    const Vec3 vec1{v1.position.x - v0.position.x, v1.position.y - v0.position.y, 0};
    const Vec3 vec2{v2.position.x - v0.position.x, v2.position.y - v0.position.y, 0};

    const float crossV1V2 = Vec3::CrossProductZ(vec1, vec2);
    const float invCrossV1V2 = 1.f / crossV1V2;

    Vec3 weight{0, 0, 0};

    for (int y = yMin; y <= yMax; ++y)
    {
        const int rowOffset = y * m_width;

        const float qy = (float) y - v0.position.y;

        for (int x = xMin; x <= xMax; ++x)
        {
            const float qx = (float) x - v0.position.x;

            const float crossQVec2 = qx * vec2.y - qy * vec2.x; // CrossProductZ(q, vec2)
            const float crossV1Q = vec1.x * qy - qx * vec1.y;   // CrossProductZ(vec1, q)

            weight.y = crossQVec2 * invCrossV1V2;
            weight.z = crossV1Q * invCrossV1V2;

            if (weight.y >= 0.f && weight.z >= 0.f && weight.y + weight.z <= 1.f)
            {
                weight.x = 1.f - weight.y - weight.z;

                const float z = Vec3::DotProduct({v0.position.z, v1.position.z, v2.position.z}, weight);

                if (z >= m_depth_buffer[x + rowOffset])
                {
                    continue;
                }

                // Perspective-correct interpolation
                float pw0 = weight.x * invW0;
                float pw1 = weight.y * invW1;
                float pw2 = weight.z * invW2;
                const float invPWSum = 1.f / (pw0 + pw1 + pw2);
                pw0 *= invPWSum;
                pw1 *= invPWSum;
                pw2 *= invPWSum;

                Color tColor{};
                if (texture == nullptr)
                {
                    tColor = Color{v0.color * pw0 + v1.color * pw1 + v2.color * pw2};
                }
                else
                {
                    const Vec2f cUV{(*uv)[0].x * pw0 + (*uv)[1].x * pw1 + (*uv)[2].x * pw2,
                                    (*uv)[0].y * pw0 + (*uv)[1].y * pw1 + (*uv)[2].y * pw2};
                    tColor = texture->Get(cUV.x, cUV.y);
                }

                const Vec3 tPos{hv0 * pw0 + hv1 * pw1 + hv2 * pw2};
                const Vec3 tNormal{hn0 * pw0 + hn1 * pw1 + hn2 * pw2};

                // light.Apply(tPos, tNormal, cam.pos,tColor);
                SetPixelColor(x, y, z, tColor);

#if 0 // Cheap wireframe
                if (min(min(pw0, pw1), pw2) < 0.016f)
                {
                    SetPixelColor(x, y, z, Colors::White);
                }
#endif

            }
        }
    }
}

inline void Rasterizer::DrawLine(const std::array<Vertex, 2>& vertices, const Mat4& transformation)
{
    // transform space: transformation * vec3      (4D)
    // clipSpace:            transformation * vec3 (4D) [-w,w]
    //      clipping out of bound triangles (0001 0010 0100)
    // NDC:  vec3/vec4.w                          (3D) [-1,1]
    //      Back face culling
    // Screen coordinate : viewport * ndc        (2D)

    std::array<Vec4, 3> transCoord;
    for (int i = 0; i < 2; i++)
    {
        transCoord[i] = view * transformation * (Vec4){vertices[i].position, 1.f};
    }

    std::array<Vec4, 3> clipCoord;
    for (int i = 0; i < 2; i++)
    {
        clipCoord[i] = projection * transCoord[i];
    }

    // clipping
    if ((clipCoord[0].x < -clipCoord[0].w || clipCoord[0].x > clipCoord[0].w || clipCoord[0].y < -clipCoord[0].w ||
         clipCoord[0].y > clipCoord[0].w || clipCoord[0].z < -clipCoord[0].w || clipCoord[0].z > clipCoord[0].w) &&
        (clipCoord[1].x < -clipCoord[1].w || clipCoord[1].x > clipCoord[1].w || clipCoord[1].y < -clipCoord[1].w ||
         clipCoord[1].y > clipCoord[1].w || clipCoord[1].z < -clipCoord[1].w || clipCoord[1].z > clipCoord[1].w))
    {
        return;
    }

    Vec3 ndc[2];
    for (int i = 0; i < 2; i++)
    {
        ndc[i] = Vec4::Homogenize(clipCoord[i]);
    }

    std::array<Vertex, 2> screenCoord;
    for (int i = 0; i < 2; i++)
    {
        screenCoord[i] = Vertex{viewport.TransformPoint(ndc[i]), vertices[i].color, vertices[i].normal};
    }
    RasterLine(screenCoord);
}

inline void Rasterizer::RasterLine(const std::array<Vertex, 2>& vertices) const
{
    Vertex v1 = vertices[0];
    Vertex v2 = vertices[1];

    const bool steep = (fabsf(v2.position.y - v1.position.y) > fabsf(v2.position.x - v1.position.x));

    if (steep)
    {
        std::swap(v1.position.x, v1.position.y);
        std::swap(v2.position.x, v2.position.y);
    }

    if (v1.position.x > v2.position.x)
    {
        std::swap(v1.position.x, v2.position.x);
        std::swap(v1.position.y, v2.position.y);
    }

    const float dx = v2.position.x - v1.position.x;
    const float dy = fabsf(v2.position.y - v1.position.y);

    float error = dx / 2.0f;
    const int ystep = (v1.position.y < v2.position.y) ? 1 : -1;
    int y = static_cast<int>(v1.position.y);

    const int maxX = static_cast<int>(v2.position.x);

    for (int x = static_cast<int>(v1.position.x); x < maxX; x++)
    {
        if (steep && std::cmp_less_equal(y, m_width) && y >= 0 && std::cmp_less_equal(x, m_height) && x >= 0)
        {
            SetPixelColor(y, x, 0, v1.color);
        }
        else if (!steep && std::cmp_less_equal(x, m_width) && x >= 0 && std::cmp_less_equal(y, m_height) && y >= 0)
        {
            SetPixelColor(x, y, 0, v1.color);
        }

        error -= dy;
        if (error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
}

void Rasterizer::ClearColorBuffer()
{
    std::fill_n((Color*) m_color_buffer->pixels, m_width * m_height, Colors::Silver);
}

inline void Rasterizer::ClearDepthBuffer()
{
    std::fill_n(m_depth_buffer, m_width * m_height, 1);
}

inline void Rasterizer::SetPixelColor(uint x, uint y, float z, const Color& c) const
{
    const uint index = x + (y * m_width);

    ((Color*) m_color_buffer->pixels)[index] = c;
    m_depth_buffer[index] = z;
}