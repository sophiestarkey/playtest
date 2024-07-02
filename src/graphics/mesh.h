#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "../geometry/collision_mesh.h"
#include "../utility/gl_wrapper.h"

struct Vertex {
    glm::vec3 m_position;
    glm::vec3 m_normal;
};

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& elements);
    Mesh(const CollisionMesh& geometry);
    Mesh(const std::string& path);

    void draw();
private:
    void init(const std::vector<Vertex>& vertices, const std::vector<GLuint>& elements);

    GL::VertexArray m_vao;
    GL::Buffer m_vbo;
    GL::Buffer m_ebo;
    GLuint m_num_elements;
};
