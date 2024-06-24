#pragma once

#include <glm/mat4x4.hpp>

#include "../utility/gl_wrapper.h"

struct Mesh {
    GL::VertexArray vao;
    GL::Buffer vbo;
    GL::Buffer ebo;
    GLuint count;
};

class MeshRenderer {
public:
    MeshRenderer();

    void begin();

    void set_projection_matrix(const glm::mat4& matrix);
    void set_view_matrix(const glm::mat4& matrix);
    void set_model_matrix(const glm::mat4& matrix);

    void draw(Mesh& mesh);
private:
    GL::Program m_program;
//  GL::Texture m_texture;

    GLint m_proj_mat_loc;
    GLint m_view_mat_loc;
    GLint m_model_mat_loc;
};
