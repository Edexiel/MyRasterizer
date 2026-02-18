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

Rasterizer::Rasterizer(uint width, uint height)
    : m_width{width}, m_height{height}
{
    m_depth_buffer = new float[width * height];
    m_color_buffer = SDL_CreateRGBSurfaceWithFormat(0, (int) width, (int) height, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_SetSurfaceBlendMode(m_color_buffer, SDL_BLENDMODE_NONE);


    // Initialize buffers
    ClearColorBuffer();
    ClearDepthBuffer();

    const float aspect = (float) width / (float) height;

    viewport = Mat4::ViewportMatrix(0.F, 0.f, (float) width, (float) height);
#if 1 // Perspective or 2D
    projection = Mat4::Perspective(60.f, aspect, 0.01f, 50.f);
#elif
    projection = Mat4::orthoMatrix(-aspect, aspect, -1.f, 1.f, 0.f, 100.f);
#endif
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

    // Vertex vertices[2]{Vertex{{0,0,0},Color{255, 255, 255}},Vertex{{1,1,1},Color{255, 255, 255}}};
    // DrawLine(vertices,view);

    // update camera matrix before transforming object
    // view = scene.GetCamera().GetCameraMatrix();

    view = Mat4::Identity();

    for (Entity& entity : scene.m_entities)
    {
        switch (entity.GetDrawMode())
        {
            // case POINT:
            // {
            //     for (uint i = 0; i < entity.mesh->vertices.size(); i++)
            //         DrawPoint(entity.mesh->vertices[i], entity.transform);
            //     break;
            // }
            case triangle: {
                if (entity.GetMesh()->indices.size() < 3)
                {
                    return;
                }

                for (uint i = 0; i < entity.GetMesh()->indices.size() - 2; i += 3)
                {
                    const std::shared_ptr<Mesh>& mesh = entity.GetMesh();
                    const std::array<Vertex, 3> triangle{mesh->vertices[mesh->indices.at(i)],
                                                         mesh->vertices[mesh->indices[i + 1]],
                                                         mesh->vertices[mesh->indices[i + 2]]};

                    if (entity.GetMesh()->uv.empty() || entity.GetMesh()->texture.IsEmpty())
                    {
                        DrawTriangle(triangle, entity.GetTransform(), scene.GetLight());
                    }
                    else
                    {
                        const std::array<Vec2f, 3> uv{entity.GetMesh()->uv[i],
                                                      entity.GetMesh()->uv[i + 1],
                                                      entity.GetMesh()->uv[i + 2]};

                        DrawTriangle(triangle, entity.GetTransform(), scene.GetLight(), &uv, &entity.GetMesh()->texture);
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

        entity.ResetTransformation();
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

    // clipping
    if ((clipCoord[0].x < -clipCoord[0].w || clipCoord[0].x > clipCoord[0].w || clipCoord[0].y < -clipCoord[0].w ||
         clipCoord[0].y > clipCoord[0].w || clipCoord[0].z < -clipCoord[0].w || clipCoord[0].z > clipCoord[0].w) &&
        (clipCoord[1].x < -clipCoord[1].w || clipCoord[1].x > clipCoord[1].w || clipCoord[1].y < -clipCoord[1].w ||
         clipCoord[1].y > clipCoord[1].w || clipCoord[1].z < -clipCoord[1].w || clipCoord[1].z > clipCoord[1].w) &&
        (clipCoord[2].x < -clipCoord[2].w || clipCoord[2].x > clipCoord[2].w || clipCoord[2].y < -clipCoord[2].w ||
         clipCoord[2].y > clipCoord[2].w || clipCoord[2].z < -clipCoord[2].w || clipCoord[2].z > clipCoord[2].w))
    {
        return;
    }

    std::array<Vec3, 3> ndc;
    for (int i = 0; i < 3; i++)
    {
        ndc[i] = Vec4::Homogenize(clipCoord[i]);
    }

    // back face culling
    if (Vec3::CrossProductZ(ndc[1] - ndc[0], ndc[2] - ndc[0]) <= 0.f)
    {
        return;
    }

    std::array<Vertex, 3> screenCoord;
    for (int i = 0; i < 3; i++)
    {
        screenCoord[i] = Vertex{viewport.TransformPoint(ndc[i]), vertices[i].color, vertices[i].normal};
    }

    Light const correctedLight{light};
    // corrected_light.Correct(view);
    // corrected_light.SetPosition(Vec3{0.f, 0.f, 0.f});

    RasterTriangle(screenCoord, transformCoordinates, clipCoord, transformNormals, correctedLight, uv, texture);
}

/**
 * @brief Rasterization function of the program
 *
 * @param vertices      pointer- screen-tranformed vertices
 * @param t_vertices    pointer- object matrix only tranformed vertices
 * @param p_vertices    pointer- non homogeneous vertices
 * @param t_normals     pointer- object matrix only transformed normals
 * @param light         Reference- light source to use
 * @param UV            Pointer- UV of texture if any (can be nullptr)
 * @param texture       Pointer- texture if any (can be nullptr)
 */
inline void Rasterizer::RasterTriangle(const std::array<Vertex, 3>& vertices,
                                       const std::array<Vec4, 3>& tVertices,
                                       const std::array<Vec4, 3>& pVertices,
                                       const std::array<Vec4, 3>& tNormals,
                                       const Light& /*light*/,
                                       const std::array<Vec2f, 3>* uv,
                                       const Texture* texture)
{
    // shortcuts
    const Vertex& v0 = vertices[0];
    const Vertex& v1 = vertices[1];
    const Vertex& v2 = vertices[2];

    const int xMin = (int) max(min(min(v0.position.x, v1.position.x), v2.position.x), 0.f);
    const int yMin = (int) max(min(min(v0.position.y, v1.position.y), v2.position.y), 0.f);
    const int xMax = (int) min(max(max(v0.position.x, v1.position.x), v2.position.x), (float) m_width - 1);
    const int yMax = (int) min(max(max(v0.position.y, v1.position.y), v2.position.y), (float) m_height - 1);

    const Vec3 vec1{v1.position.x - v0.position.x, v1.position.y - v0.position.y, 0};
    const Vec3 vec2{v2.position.x - v0.position.x, v2.position.y - v0.position.y, 0};

    Vec3 weight{0, 0, 0};

    for (int y = yMin; y <= yMax; ++y)
    {
        for (int x = xMin; x <= xMax; ++x)
        {
            const Vec3 q{(float)x - v0.position.x, (float)y - v0.position.y, 0};
            // const Vec3 q{x - v1.position.x, y - v1.position.y, 0};

            weight.y = Vec3::CrossProductZ(q, vec2) / Vec3::CrossProductZ(vec1, vec2);
            weight.z = Vec3::CrossProductZ(vec1, q) / Vec3::CrossProductZ(vec1, vec2);

            if (weight.y >= 0.f && weight.z >= 0.f && weight.y + weight.z <= 1)
            {
                weight.x = 1.f - weight.y - weight.z;

                const float z = Vec3::DotProduct({v0.position.z, v1.position.z, v2.position.z}, weight);

                // if (z > depth_buffer[x + y * width])
                // {
                //     continue;
                // }

                weight.x /= pVertices[0].w;
                weight.y /= pVertices[1].w;
                weight.z /= pVertices[2].w;

                weight = weight * (1 / (weight.x + weight.y + weight.z));

                const Vec3 hv0 = Vec4::Homogenize(tVertices[0]);
                const Vec3 hv1 = Vec4::Homogenize(tVertices[1]);
                const Vec3 hv2 = Vec4::Homogenize(tVertices[2]);

                const Vec3 tPos{hv0 * weight.x + hv1 * weight.y + hv2 * weight.z};

                const Vec3 hn0 = Vec4::Homogenize(tNormals[0]);
                const Vec3 hn1 = Vec4::Homogenize(tNormals[1]);
                const Vec3 hn2 = Vec4::Homogenize(tNormals[2]);

                const Vec3 tNormal{hn0 * weight.x + hn1 * weight.y + hn2 * weight.z};

#if 1 // Cheap wireframe
                if (min(min(weight.x, weight.y), weight.z) < 0.016f)
                {
                    SetPixelColor(x, y, z, {(unsigned char) (255), (unsigned char) (255), (unsigned char) (255)});
                }
#endif

                Color tColor{};
                if (texture == nullptr || uv == nullptr)
                {
                    tColor = {v0.color * weight.x + v1.color * weight.y + v2.color * weight.z};
                }
                else
                {
                    const Vec2f cUV{(uv->at(0).x * weight.x) + (uv->at(1).x * weight.y) + (uv->at(2).x * weight.z),
                                    (uv->at(0).y * weight.x) + (uv->at(1).y * weight.y) + (uv->at(2).y * weight.z)};
                    tColor = texture->Accessor(cUV.x, cUV.y);
                }

                // light.apply_light(t_pos, t_normal, t_color);
                SetPixelColor(x, y, z, tColor);
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
    for (short i = 0; i < 2; i++)
    {
        clipCoord[i] = projection * transCoord[i];
    }

    // clipping
    // if ((clipCoord[0].x < -clipCoord[0].w || clipCoord[0].x > clipCoord[0].w || clipCoord[0].y < -clipCoord[0].w ||
    //         clipCoord[0].y > clipCoord[0].w || clipCoord[0].z < -clipCoord[0].w || clipCoord[0].z > clipCoord[0].w) &&
    //     (clipCoord[1].x < -clipCoord[1].w || clipCoord[1].x > clipCoord[1].w || clipCoord[1].y < -clipCoord[1].w ||
    //         clipCoord[1].y > clipCoord[1].w || clipCoord[1].z < -clipCoord[1].w || clipCoord[1].z > clipCoord[1].w))
    // {
    //     return;
    // }

    Vec3 ndc[2];
    for (int i = 0; i < 2; i++)
    {
        ndc[i] = Vec4::Homogenize(clipCoord[i]);
    }

    // back face culling
    if (Vec3::CrossProductZ(ndc[1], ndc[0]) <= 0.f)
    {
        return;
    }

    Vertex screenCoord[2];
    for (int i = 0; i < 2; i++)
    {
        screenCoord[i] = Vertex{viewport.TransformPoint(ndc[i]), vertices[i].color, vertices[i].normal};
    }
    RasterLine(screenCoord);
}

inline void Rasterizer::RasterLine(const Vertex* vertex)
{
    Vertex v1 = vertex[0];
    Vertex v2 = vertex[1];

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
    int y = (int) v1.position.y;

    const int maxX = (int) v2.position.x;

    for (int x = (int) v1.position.x; x < maxX; x++)
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

void Rasterizer::ClearColorBuffer() const
{
    static const Color gray{150, 150, 150, 255};
    std::fill_n((Color*) m_color_buffer->pixels, m_width * m_height, gray);
}

inline void Rasterizer::ClearDepthBuffer() const
{
    std::fill_n(m_depth_buffer, m_width * m_height, FLT_MAX);
}

inline void Rasterizer::SetPixelColor(uint x, uint y, float z, const Color& c) const
{
    const uint index = x + (y * m_width);

    ((Color*) m_color_buffer->pixels)[index] = c;
    // colorBuffer[index] = c;
    m_depth_buffer[index] = z;
}

// void Rasterizer::draw_point(Vertex v, Mat4 &transformation)
// {
//     Mat4 mat_finale = viewport * projection * transformation;
//     v.position = (mat_finale * Vec4{v.position, 1}).xyz;
//     // viewport
//     set_pixel_color((uint)v.position.x, (uint)v.position.y, 0, v.color);
// }