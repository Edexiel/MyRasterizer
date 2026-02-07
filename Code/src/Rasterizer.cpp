#include "Rasterizer.hpp"

#include <cmath>
#include <iostream>

#include <GL/gl.h>
#include <SDL.h>
#include "Mat4.hpp"
#include "Tools.hpp"
#include "Vec2.hpp"

#include "Camera.hpp"

Rasterizer::Rasterizer(uint width, uint height) : width{width}, height{height}
{
    colorBuffer = SDL_CreateRGBSurfaceWithFormat(0, (int)width, (int)height, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_SetSurfaceBlendMode(colorBuffer, SDL_BLENDMODE_NONE);

    // color_buffer = new Color[width * height];
    depth_buffer = new float[width * height];

    // Initialize buffers
    ClearColorBuffer();
    ClearDepthBuffer();

    // //Generate the texture to be sent to the GPU
    // glGenTextures(1, &color_buffer_texture);
    //
    // //Upload an empty screen to the GPU (?)
    // UploadTexture();
    //
    // //Screen texture parameters
    // glBindTexture(GL_TEXTURE_2D, color_buffer_texture);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //important
    // glBindTexture(GL_TEXTURE_2D, 0);

    // set the view mode

    const float aspect = (float)width / (float)height;

    viewport = Mat4::ViewportMatrix(0.f, 0.f, (float)width, (float)height);
#if 1 // Perspective or 2D
    projection = Mat4::Perspective(60.f, aspect, 0.01f, 50.f);
#elif
    projection = Mat4::orthoMatrix(-aspect, aspect, -1.f, 1.f, 0.f, 100.f);
#endif
}

Rasterizer::~Rasterizer()
{
    SDL_FreeSurface(colorBuffer);
}

SDL_Surface* Rasterizer::GetColorBuffer()
{
    return colorBuffer;
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

    for (Entity& entity : scene.entities)
    {
        switch (entity.GetDrawMode())
        {
        // case POINT:
        // {
        //     for (uint i = 0; i < entity.mesh->vertices.size(); i++)
        //         DrawPoint(entity.mesh->vertices[i], entity.transform);
        //     break;
        // }
        case TRIANGLE:
            {
                if (entity.GetMesh()->indices.size() < 3)
                {
                    return;
                }

                for (uint i = 0; i < entity.GetMesh()->indices.size() - 2; i += 3)
                {
                    const std::shared_ptr<Mesh>& Mesh = entity.GetMesh();
                    const Vertex triangle[3]{
                        Mesh->vertices[Mesh->indices[i]],
                        Mesh->vertices[Mesh->indices[i + 1]],
                        Mesh->vertices[Mesh->indices[i + 2]]
                    };

                    if (entity.GetMesh()->UV.empty() || entity.GetMesh()->texture.IsEmpty())
                    {
                        DrawTriangle(triangle, entity.GetTransform(), scene.GetLight());
                    }
                    else
                    {
                        Vec2f uv[3]{entity.GetMesh()->UV[i], entity.GetMesh()->UV[i + 1], entity.GetMesh()->UV[i + 2]};

                        DrawTriangle(triangle, entity.GetTransform(), scene.GetLight(), uv, &entity.GetMesh()->texture);
                    }
                }

                break;
            }
        case LINE:
            {
                if (entity.GetMesh()->indices.size() < 2)
                    break;

                for (uint i = 0; i < entity.GetMesh()->indices.size() - 1; i += 1)
                {
                    Vertex line[2]{
                        entity.GetMesh()->vertices[entity.GetMesh()->indices[i]],
                        entity.GetMesh()->vertices[entity.GetMesh()->indices[i + 1]]
                    };

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

/**
 * @brief Entry point of the rasterization of a triangle
 *
 * @param vertices
 * @param transformation
 * @param light
 */
inline void Rasterizer::DrawTriangle(const Vertex(&vertices)[3], const Mat4& transformation, const Light& light,
                                     const Vec2f* UV, const Texture* texture) const
{
    // transform space: transformation * vec3      (4D)
    // clipSpace:            transformation * vec3 (4D) [-w,w]
    //      clipping out of bound triangles (0001 0010 0100)
    // NDC:  vec3/vec4.w                          (3D) [-1,1]
    //      Back face culling
    // Screen coordinate : viewport * ndc        (2D)

    Vec4 transformCoordinates[3];
    Vec4 transformNormals[3];

    for (int i = 0; i < 3; i++)
    {
        transformCoordinates[i] = view * transformation * Vec4{vertices[i].position, 1.f};
        transformNormals[i] = transformation * Vec4{vertices[i].normal, 0.f};
    }

    Vec4 clipCoord[3];
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

    Vec3 ndc[3];
    for (int i = 0; i < 3; i++)
    {
        ndc[i] = Vec4::Homogenize(clipCoord[i]);
    }

    // back face culling
    if (Vec3::CrossProductZ(ndc[1] - ndc[0], ndc[2] - ndc[0]) <= 0.f)
    {
        return;
    }

    Vertex screenCoord[3];
    for (int i = 0; i < 3; i++)
    {
        screenCoord[i] = Vertex{viewport.TransformPoint(ndc[i]), vertices[i].color, vertices[i].normal};
    }

    Light corrected_light{light};
    // corrected_light.Correct(view);
    // corrected_light.SetPosition(Vec3{0.f, 0.f, 0.f});

    RasterTriangle(screenCoord, transformCoordinates, clipCoord, transformNormals, corrected_light, UV, texture);
}

/**
 * @brief Rasterization function of the program
 *
 * @param vertices      pointer- screen-tranformed vertices
 * @param t_vertices     pointer- object matrix only tranformed vertices
 * @param p_vertices    pointer- non homogeneous vertices
 * @param t_normals     pointer- object matrix only transformed normals
 * @param light         Reference- light source to use
 * @param UV            Pointer- UV of texture if any (can be nullptr)
 * @param texture       Pointer- texture if any (can be nullptr)
 */
inline void Rasterizer::RasterTriangle(const Vertex(&vertices)[3], const Vec4(&t_vertices)[3], const Vec4(&p_vertices)[3], const Vec4(&t_normals)[3], const Light& light, const Vec2f* UV , const Texture* texture) const
{
    // shortcuts
    const Vertex& v1 = vertices[0];
    const Vertex& v2 = vertices[1];
    const Vertex& v3 = vertices[2];

    const int xMin = (int)max(min(min(v1.position.x, v2.position.x), v3.position.x), 0.f);
    const int yMin = (int)max(min(min(v1.position.y, v2.position.y), v3.position.y), 0.f);
    const int xMax = (int)min(max(max(v1.position.x, v2.position.x), v3.position.x), (float)width - 1);
    const int yMax = (int)min(max(max(v1.position.y, v2.position.y), v3.position.y), (float)height - 1);

    const Vec3 vec1{v2.position.x - v1.position.x, v2.position.y - v1.position.y, 0};
    const Vec3 vec2{v3.position.x - v1.position.x, v3.position.y - v1.position.y, 0};

    Vec3 weight{0, 0, 0};

    for (int y = yMin; y <= yMax; ++y)
    {
        for (int x = xMin; x <= xMax; ++x)
        {
            // const Vec3 q{(float)x - v1.position.x, (float)y - v1.position.y, 0};
            const Vec3 q{x - v1.position.x, y - v1.position.y, 0};

            weight.y = Vec3::CrossProductZ(q, vec2) / Vec3::CrossProductZ(vec1, vec2);
            weight.z = Vec3::CrossProductZ(vec1, q) / Vec3::CrossProductZ(vec1, vec2);

            if (weight.y >= 0.f && weight.z >= 0.f && weight.y + weight.z <= 1)
            {
                weight.x = 1.f - weight.y - weight.z;

                const float z = Vec3::DotProduct({v1.position.z, v2.position.z, v3.position.z}, weight);

                // if (z > depth_buffer[x + y * width])
                // {
                //     continue;
                // }

                weight.x /= p_vertices[0].w;
                weight.y /= p_vertices[1].w;
                weight.z /= p_vertices[2].w;

                weight = weight * (1 / (weight.x + weight.y + weight.z));

                const Vec3 hv0 = Vec4::Homogenize(t_vertices[0]);
                const Vec3 hv1 = Vec4::Homogenize(t_vertices[1]);
                const Vec3 hv2 = Vec4::Homogenize(t_vertices[2]);

                const Vec3 t_pos
                {
                    hv0 * weight.x + hv1 * weight.y + hv2* weight.z
                };

                const Vec3 hn0 = Vec4::Homogenize(t_normals[0]);
                const Vec3 hn1 = Vec4::Homogenize(t_normals[1]);
                const Vec3 hn2 = Vec4::Homogenize(t_normals[2]);

                const Vec3 t_normal
                {
                    hn0 * weight.x + hn1 * weight.y + hn2 * weight.z
                };

#if 1 // Cheap wireframe
                    if (min(min(weight.x, weight.y), weight.z) < 0.016f)
                    {
                        SetPixelColor(x, y, z, {(unsigned char)(255), (unsigned char)(255), (unsigned char)(255)});
                    }
#endif

                Color t_color{};
                if (texture == nullptr || UV == nullptr)
                {
                    t_color = {v1.color * weight.x + v2.color * weight.y + v3.color * weight.z};
                }
                else
                {
                    const Vec2f c_uv
                    {
                        UV[0].x * weight.x + UV[1].x * weight.y + UV[2].x * weight.z,
                        UV[0].y * weight.x + UV[1].y * weight.y + UV[2].y * weight.z
                    };
                    t_color = texture->Accessor(c_uv.x, c_uv.y);
                }

                // light.apply_light(t_pos, t_normal, t_color);
                SetPixelColor(x, y, z, t_color);
            }
        }
    }
}

inline void Rasterizer::DrawLine(const Vertex(&vertices)[2], const Mat4& transformation)
{
    // transform space: transformation * vec3      (4D)
    // clipSpace:            transformation * vec3 (4D) [-w,w]
    //      clipping out of bound triangles (0001 0010 0100)
    // NDC:  vec3/vec4.w                          (3D) [-1,1]
    //      Back face culling
    // Screen coordinate : viewport * ndc        (2D)

    Vec4 transCoord[2];
    for (int i = 0; i < 2; i++)
    {
        transCoord[i] = view * transformation * (Vec4){vertices[i].position, 1.f};
    }

    Vec4 clipCoord[2];
    for (short i = 0; i < 2; i++)
        clipCoord[i] = projection * transCoord[i];

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
        return;

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
    int y = (int)v1.position.y;

    const int maxX = (int)v2.position.x;

    for (int x = (int)v1.position.x; x < maxX; x++)
    {
        if (steep && y <= (int)width && y >= 0 && x <= (int)height && x >= 0)
        {
            SetPixelColor(y, x, 0, v1.color);
        }
        else if (!steep && x <= (int)width && x >= 0 && y <= (int)height && y >= 0)
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
    std::fill_n((Color*)colorBuffer->pixels, width * height, gray);
}

inline void Rasterizer::ClearDepthBuffer() const
{
    std::fill_n(depth_buffer, width * height, FLT_MAX);
    // memset(depth_buffer,,width * height * sizeof(float));
    // const uint size = width * height;
    //
    // for (size_t i = 0; i < size; i++)
    // {
    //     depth_buffer[i] = 10.f;
    // }
}

inline void Rasterizer::SetPixelColor(uint x, uint y, float z, const Color& c) const
{
    const uint index = x + y * width;

    ((Color*)colorBuffer->pixels)[index] = c;
    // colorBuffer[index] = c;
    depth_buffer[index] = z;
}

// void Rasterizer::draw_point(Vertex v, Mat4 &transformation)
// {
//     Mat4 mat_finale = viewport * projection * transformation;
//     v.position = (mat_finale * Vec4{v.position, 1}).xyz;
//     // viewport
//     set_pixel_color((uint)v.position.x, (uint)v.position.y, 0, v.color);
// }
