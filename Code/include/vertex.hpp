#pragma once

#include "color.hpp"
#include "vec3.hpp"

class Vertex
{
public:
    Vec3 position = Vec3::Zero();
    Color color{};
    Vec3 normal = Vec3::Zero();

    // Vertex(const Vertex& other) = default;
    Vertex() = default;

    Vertex(const Vec3& position, const Color& color) : position{position}, color{color}, normal(Vec3::Zero()) {}

    Vertex(const Vec3& position, const Color& color, const Vec3& normal)
        : position{position}, color{color}, normal{normal}
    {
    }
};