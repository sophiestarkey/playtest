#include <glm/gtc/type_ptr.hpp>

#include "../utility/file_io.h"
#include "mesh_shader.h"

MeshShader::MeshShader()
{
    GL::Shader vertex_shader(GL_VERTEX_SHADER);
    GL::ShaderSource(vertex_shader, ReadFile("res/shaders/basic_rim.vert"));
    GL::CompileShader(vertex_shader);

    GL::Shader fragment_shader(GL_FRAGMENT_SHADER);
    GL::ShaderSource(fragment_shader, ReadFile("res/shaders/basic_rim.frag"));
    GL::CompileShader(fragment_shader);

    glAttachShader(m_program, vertex_shader);
    glAttachShader(m_program, fragment_shader);
    GL::LinkProgram(m_program);
    glDetachShader(m_program, vertex_shader);
    glDetachShader(m_program, fragment_shader);

    m_proj_mat_loc = glGetUniformLocation(m_program, "u_projection_mat");
    m_view_mat_loc = glGetUniformLocation(m_program, "u_view_mat");
    m_model_mat_loc = glGetUniformLocation(m_program, "u_model_mat");
    m_color_loc = glGetUniformLocation(m_program, "u_color");
}

void MeshShader::use()
{
    glUseProgram(m_program);
}

void MeshShader::set_projection_matrix(const glm::mat4& matrix)
{
    glUniformMatrix4fv(m_proj_mat_loc, 1, false, glm::value_ptr(matrix));
}

void MeshShader::set_view_matrix(const glm::mat4& matrix)
{
    glUniformMatrix4fv(m_view_mat_loc, 1, false, glm::value_ptr(matrix));
}
void MeshShader::set_model_matrix(const glm::mat4& matrix)
{
    glUniformMatrix4fv(m_model_mat_loc, 1, false, glm::value_ptr(matrix));
}

void MeshShader::set_color(const glm::vec3& color)
{
    glUniform3fv(m_color_loc, 1, glm::value_ptr(color));
}