#include "mesh.hpp"

#include <cmath>
#include <memory>
#include <string>

#include "color.hpp"
#include "vec2.hpp"
#include "vertex.hpp"

// #define TINYOBJLOADER_IMPLEMENTATION
// #include "tiny_obj_loader.h"

std::shared_ptr<Mesh> Mesh::CreateTexCube(const std::string& filename, const Color& color)
{
    const Vec2f topLeft{0, 0};
    const Vec2f topRight{1, 0};
    const Vec2f bottomLeft{0, 1};
    const Vec2f bottomRight{1, 1};

    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

    mesh->texture.Load_PNG(filename.c_str());

    mesh->vertices.push_back(Vertex{{-0.5, -0.5, 0.5}, color});
    mesh->vertices.push_back(Vertex{{0.5, -0.5, 0.5}, color});
    mesh->vertices.push_back(Vertex{{-0.5, -0.5, -0.5}, color});
    mesh->vertices.push_back(Vertex{{0.5, -0.5, -0.5}, color});
    mesh->vertices.push_back(Vertex{{0.5, 0.5, -0.5}, color});
    mesh->vertices.push_back(Vertex{{0.5, 0.5, 0.5}, color});
    mesh->vertices.push_back(Vertex{{-0.5, 0.5, 0.5}, color});
    mesh->vertices.push_back(Vertex{{-0.5, 0.5, -0.5}, color});

    for (Vertex& vertex : mesh->vertices)
    {
        vertex.normal = Vec3::Normalize(vertex.position);
    }

    // down
    mesh->indices.push_back(1);
    mesh->uv.push_back(bottomLeft);
    mesh->indices.push_back(0);
    mesh->uv.push_back(bottomRight);
    mesh->indices.push_back(2);
    mesh->uv.push_back(topRight);
    mesh->indices.push_back(1);
    mesh->uv.push_back(bottomLeft);
    mesh->indices.push_back(2);
    mesh->uv.push_back(topRight);
    mesh->indices.push_back(3);
    mesh->uv.push_back(topLeft);

    // right side
    mesh->indices.push_back(3);
    mesh->uv.push_back(bottomRight);
    mesh->indices.push_back(4);
    mesh->uv.push_back(topRight);
    mesh->indices.push_back(1);
    mesh->uv.push_back(bottomLeft);
    mesh->indices.push_back(1);
    mesh->uv.push_back(bottomLeft);
    mesh->indices.push_back(4);
    mesh->uv.push_back(topRight);
    mesh->indices.push_back(5);
    mesh->uv.push_back(topLeft);

    // front
    mesh->indices.push_back(1);
    mesh->uv.push_back(bottomRight);
    mesh->indices.push_back(5);
    mesh->uv.push_back(topRight);
    mesh->indices.push_back(0);
    mesh->uv.push_back(bottomLeft);
    mesh->indices.push_back(0);
    mesh->uv.push_back(bottomLeft);
    mesh->indices.push_back(5);
    mesh->uv.push_back(topRight);
    mesh->indices.push_back(6);
    mesh->uv.push_back(topLeft);

    // left side
    mesh->indices.push_back(0);
    mesh->uv.push_back(bottomRight);
    mesh->indices.push_back(6);
    mesh->uv.push_back(topRight);
    mesh->indices.push_back(2);
    mesh->uv.push_back(bottomLeft);
    mesh->indices.push_back(2);
    mesh->uv.push_back(bottomLeft);
    mesh->indices.push_back(6);
    mesh->uv.push_back(topRight);
    mesh->indices.push_back(7);
    mesh->uv.push_back(topLeft);

    // back
    mesh->indices.push_back(2);
    mesh->uv.push_back(bottomRight);
    mesh->indices.push_back(7);
    mesh->uv.push_back(topRight);
    mesh->indices.push_back(4);
    mesh->uv.push_back(topLeft);
    mesh->indices.push_back(3);
    mesh->uv.push_back(bottomLeft);
    mesh->indices.push_back(2);
    mesh->uv.push_back(bottomRight);
    mesh->indices.push_back(4);
    mesh->uv.push_back(topLeft);

    // top
    mesh->indices.push_back(4);
    mesh->uv.push_back(topRight);
    mesh->indices.push_back(7);
    mesh->uv.push_back(topLeft);
    mesh->indices.push_back(6);
    mesh->uv.push_back(bottomLeft);
    mesh->indices.push_back(4);
    mesh->uv.push_back(topRight);
    mesh->indices.push_back(6);
    mesh->uv.push_back(bottomLeft);
    mesh->indices.push_back(5);
    mesh->uv.push_back(bottomRight);

    return mesh;
}

std::shared_ptr<Mesh> Mesh::CreateCube(const Color& color)
{
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

    mesh->vertices.push_back(Vertex{{-0.5, -0.5, 0.5}, color});
    mesh->vertices.push_back(Vertex{{0.5, -0.5, 0.5}, color});
    mesh->vertices.push_back(Vertex{{-0.5, -0.5, -0.5}, color});
    mesh->vertices.push_back(Vertex{{0.5, -0.5, -0.5}, color});
    mesh->vertices.push_back(Vertex{{0.5, 0.5, -0.5}, color});
    mesh->vertices.push_back(Vertex{{0.5, 0.5, 0.5}, color});
    mesh->vertices.push_back(Vertex{{-0.5, 0.5, 0.5}, color});
    mesh->vertices.push_back(Vertex{{-0.5, 0.5, -0.5}, color});

    for (Vertex& vertex : mesh->vertices)
    {
        vertex.normal = Vec3::Normalize(vertex.position);
    }

    // down
    mesh->indices.push_back(1);
    mesh->indices.push_back(0);
    mesh->indices.push_back(2);
    mesh->indices.push_back(1);
    mesh->indices.push_back(2);
    mesh->indices.push_back(3);

    // right side
    mesh->indices.push_back(3);
    mesh->indices.push_back(4);
    mesh->indices.push_back(1);
    mesh->indices.push_back(1);
    mesh->indices.push_back(4);
    mesh->indices.push_back(5);

    // front
    mesh->indices.push_back(1);
    mesh->indices.push_back(5);
    mesh->indices.push_back(0);
    mesh->indices.push_back(0);
    mesh->indices.push_back(5);
    mesh->indices.push_back(6);

    // left side
    mesh->indices.push_back(0);
    mesh->indices.push_back(6);
    mesh->indices.push_back(2);
    mesh->indices.push_back(2);
    mesh->indices.push_back(6);
    mesh->indices.push_back(7);

    // back
    mesh->indices.push_back(2);
    mesh->indices.push_back(7);
    mesh->indices.push_back(4);
    mesh->indices.push_back(3);
    mesh->indices.push_back(2);
    mesh->indices.push_back(4);

    // top
    mesh->indices.push_back(4);
    mesh->indices.push_back(7);
    mesh->indices.push_back(6);
    mesh->indices.push_back(4);
    mesh->indices.push_back(6);
    mesh->indices.push_back(5);

    return mesh;
}

std::shared_ptr<Mesh> Mesh::CreateTriangle()
{
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

    const Vertex v1{{0.5, -0.5, 0}, {0, 255, 0, 255}};
    const Vertex v2{{0, 0.5, 0}, {255, 0, 0, 255}};
    const Vertex v3{{-0.5, -0.5, 0}, {0, 0, 255, 255}};

    mesh->vertices.push_back(v1);
    mesh->vertices.push_back(v2);
    mesh->vertices.push_back(v3);

    for (Vertex& vertex : mesh->vertices)
    {
        vertex.normal = Vec3::Normalize(vertex.position);
    }

    mesh->indices.push_back(0);
    mesh->indices.push_back(1);
    mesh->indices.push_back(2);

    return mesh;
}

std::shared_ptr<Mesh> Mesh::CreateSphere(int latitudeCount, int longitudeCount, Color color)
{
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

    for (int i = 0; i < latitudeCount; i++)
    {
        for (int j = 0; j < longitudeCount + 1; j++)
        {
            const float r = sinf((float) i * std::numbers::pi_v<float> / (float) latitudeCount);
            mesh->vertices.push_back(Vertex{{cosf((float) j * (M_PIf * 2) / (float) longitudeCount) * r,
                                             cosf((float) i * M_PIf / (float) latitudeCount),
                                             sinf((float) j * (M_PIf * 2) / (float) longitudeCount) * r},
                                            color});
        }
    }
    mesh->vertices.push_back({{0, -1, 0}, color});

    const int iMax = (int) mesh->vertices.size() - 1;

    for (Vertex& vertex : mesh->vertices)
    {
        vertex.normal = Vec3::Normalize(vertex.position);
    }

    for (int i = 0; i < iMax; i++)
    {
        int const index = i + longitudeCount + 1 > iMax ? iMax : i + longitudeCount + 1;
        int const indeX2 = i + longitudeCount > iMax ? iMax : i + longitudeCount;
        mesh->indices.push_back(i);
        mesh->indices.push_back(1 + i);
        mesh->indices.push_back(index);

        mesh->indices.push_back(indeX2);
        mesh->indices.push_back(i);
        mesh->indices.push_back(index);
    }

    return mesh;
}

std::shared_ptr<Mesh> Mesh::CreateVectorLight(float x, float y, float z)
{
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
    mesh->vertices.push_back(Vertex{{x, y, z}, {255, 0, 0}});
    mesh->vertices.push_back(Vertex{{0, 0, 0}, {0, 0, 0}});
    mesh->indices.push_back(0);
    mesh->indices.push_back(1);

    return mesh;
}

// std::shared_ptr<Mesh> Mesh::LoadObj(std::string& path)
// {
//     std::shared_ptr<Mesh> object = new Mesh{};
//
//     bool hasNormals = false;
//     bool hasColors = false;
//
//     std::string warn;
//     std::string err;
//
//     tinyobj::attrib_t attrib;
//     std::vector<tinyobj::shape_t> shapes;
//
//     tinyobj::LoadObj(&attrib, &shapes, NULL, &warn, &err, path.c_str(), NULL, true);
//
//     if (!err.empty())
//         std::cout << "Error loading obj: " << err.c_str() << std::endl;
//     if (!warn.empty())
//         std::cout << "Warning loading obj: " << warn.c_str() << std::endl;
//
//     hasNormals = !attrib.normals.empty();
//     hasColors = !attrib.colors.empty();
//
//     for (uint i = 0; i < attrib.vertices.size() - 2; i += 3)
//     {
//         Vertex v{};
//
//         v.position = {
//             (float)attrib.vertices[i],
//             (float)attrib.vertices[i + 1],
//             (float)attrib.vertices[i + 2]
//         };
//
//         v.normal = hasNormals
//                        ? Vec3{
//                            (float)attrib.normals[i],
//                            (float)attrib.normals[i + 1],
//                            (float)attrib.normals[i + 2]
//                        }
//                        : Vec3{0.f, 0.f, 0.f};
//
//         v.color = hasColors
//                       ? Color{
//                           (unsigned char)clamp((float)attrib.colors[i] * 255, 0.f, 255.f),
//                           (unsigned char)clamp((float)attrib.colors[i + 1] * 255, 0.f, 255.f),
//                           (unsigned char)clamp((float)attrib.colors[i + 2] * 255, 0.f, 255.f)
//                       }
//                       : Color{255, 255, 255};
//
//         object->vertices.push_back(v);
//     }
//
//     for (tinyobj::shape_t& shape : shapes)
//     {
//         for (tinyobj::index_t& index : shape.mesh.indices)
//         {
//             object->indices.push_back(index.vertex_index);
//         }
//     }
//
//     if (!hasNormals)
//     {
//         for (uint i = 0; i < object->vertices.size(); i++)
//         {
//             object->vertices[i].normal = Vec3::Normalize(object->vertices[i].position);
//         }
//     }
//
//     return object;
// }
