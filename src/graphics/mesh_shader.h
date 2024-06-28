#pragma once

#include <glm/mat4x4.hpp>

#include "../utility/gl_wrapper.h"

class MeshShader {
public:
    MeshShader();

    void use();

    void set_projection_matrix(const glm::mat4& matrix);
    void set_view_matrix(const glm::mat4& matrix);
    void set_model_matrix(const glm::mat4& matrix);
    void set_color(const glm::vec3& color);
private:
    GL::Program m_program;

    GLint m_proj_mat_loc;
    GLint m_view_mat_loc;
    GLint m_model_mat_loc;
    GLint m_color_loc;
};
