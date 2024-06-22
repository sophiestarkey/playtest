#include "../utility/file_io.h"
#include "mesh_renderer.h"

MeshRenderer::MeshRenderer()
{
    GL::Shader vertex_shader(GL_VERTEX_SHADER);
    GL::ShaderSource(vertex_shader, ReadFile("res/shaders/matcap_vert.glsl"));
    GL::CompileShader(vertex_shader);

    GL::Shader fragment_shader(GL_FRAGMENT_SHADER);
    GL::ShaderSource(fragment_shader, ReadFile("res/shaders/matcap_frag.glsl"));
    GL::CompileShader(fragment_shader);

    glAttachShader(m_program, vertex_shader);
    glAttachShader(m_program, fragment_shader);
    GL::LinkProgram(m_program);
    glDetachShader(m_program, vertex_shader);
    glDetachShader(m_program, fragment_shader);

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Image image("res/textures/matcap_normal.png", ImageFormat::RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.data());
}