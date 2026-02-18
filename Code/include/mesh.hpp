#pragma once

#include <memory>
#include <string>
#include <vector>

#include "color.hpp"
#include "texture.hpp"
#include "vec2.hpp"
#include "vertex.hpp"

class Mesh
{
public:
    Mesh() = default;
    Mesh(Mesh& other) = default;
    Mesh(const Mesh& other) = default;
    Mesh(Mesh&& other) noexcept = default;
    Mesh& operator=(const Mesh& other) = default;
    Mesh& operator=(Mesh&& other) noexcept = default;

    ~Mesh() = default;

    std::vector<Vertex> vertices;
    std::vector<int> indices;
    std::vector<Vec2f> uv;

    Texture texture;

    static std::shared_ptr<Mesh> CreateTriangle();
    static std::shared_ptr<Mesh> CreateTexCube(const std::string& filename, const t_Color<unsigned char>& color);
    static std::shared_ptr<Mesh> CreateCube(const t_Color<unsigned char>& color);
    static std::shared_ptr<Mesh> CreateSphere(int latitudeCount, int longitudeCount, Color = {255, 255, 255});
    static std::shared_ptr<Mesh> CreateVectorLight(float x, float y, float z);
    // static std::shared_ptr<Mesh> LoadObj(std::string& filename);
};
