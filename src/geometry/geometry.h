#pragma once

#include <array>
#include <glm/vec3.hpp>

struct Triangle {
    Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);

    std::array<glm::vec3, 3> points;
};
