#pragma once

#include <array>
#include <glm/vec3.hpp>
#include <string>
#include <vector>

struct Triangle {
    std::array<glm::vec3, 3> m_vertices;
};

class CollisionMesh {
public:
    CollisionMesh(const std::string& path);

    const std::vector<Triangle>& triangles() const;
private:
    std::vector<Triangle> m_triangles;
};
