#include <glm/gtc/type_ptr.hpp>

#include "../utility/file_io.h"
#include "mesh_renderer.h"

MeshRenderer::MeshRenderer()
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

//  glBindTexture(GL_TEXTURE_2D, m_texture);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//  Image image("res/textures/matcap_basic.png", ImageFormat::RGB);
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.data());
//  glGenerateMipmap(GL_TEXTURE_2D);
}

void MeshRenderer::begin()
{
    glUseProgram(m_program);
//  glBindTexture(GL_TEXTURE_2D, m_texture);
}

void MeshRenderer::set_projection_matrix(const glm::mat4& matrix)
{
    glUniformMatrix4fv(m_proj_mat_loc, 1, false, glm::value_ptr(matrix));
}

void MeshRenderer::set_view_matrix(const glm::mat4& matrix)
{
    glUniformMatrix4fv(m_view_mat_loc, 1, false, glm::value_ptr(matrix));
}
void MeshRenderer::set_model_matrix(const glm::mat4& matrix)
{
    glUniformMatrix4fv(m_model_mat_loc, 1, false, glm::value_ptr(matrix));
}

void MeshRenderer::draw(Mesh& mesh)
{
    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES, mesh.count, GL_UNSIGNED_INT, (void*)0);
}