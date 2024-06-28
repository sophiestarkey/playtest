#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "../geometry/geometry.h"
#include "../utility/gl_wrapper.h"

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& elements);
    Mesh(const Geometry& geometry, bool use_face_normals = false);
    Mesh(const std::string& path);

    void draw();
private:
    void init(const std::vector<Vertex>& vertices, const std::vector<GLuint>& elements);

    GL::VertexArray m_vao;
    GL::Buffer m_vbo;
    GL::Buffer m_ebo;
    GLuint m_num_elements;
};
