#pragma once

#include <array>
#include <glm/vec3.hpp>
#include <string>
#include <vector>

#include "geometry.h"

class CollisionMesh {
public:
    CollisionMesh(const std::string& path);

    const std::vector<Triangle>& triangles() const;
private:
    std::vector<Triangle> m_triangles;
};
