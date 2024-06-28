#pragma once

#include <glm/vec3.hpp>
#include <string>
#include <vector>

struct Vertex {
    glm::vec3 m_position;
    glm::vec3 m_normal;
};

struct Face {
    glm::vec3 m_normal;
    unsigned m_num_indices;
    unsigned* m_indices;
};

class Geometry {
public:
    Geometry(const std::string& path);

    const std::vector<Vertex>& vertices() const;
    const std::vector<Face>& faces() const;
private:
    std::vector<Vertex> m_vertices;
    std::vector<Face> m_faces;
    std::vector<unsigned> m_indices;
};
