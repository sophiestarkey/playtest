#include <stdexcept>
#include <utility>

#include "gl_wrapper.h"

namespace GL {
    Buffer::Buffer()
    {
        glGenBuffers(1, &m_id);
    }

    Buffer::~Buffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    Buffer::Buffer(Buffer&& rhs) noexcept : m_id(std::exchange(rhs.m_id, 0))
    {
    }

    Buffer& Buffer::operator=(Buffer&& rhs) noexcept
    {
        if (this != &rhs) {
            glDeleteBuffers(1, &m_id);
            m_id = std::exchange(rhs.m_id, 0);
        }

        return *this;
    }

    Buffer::operator GLuint() const
    {
        return m_id;
    }

    Program::Program() : m_id(glCreateProgram())
    {
    }

    Program::~Program()
    {
        glDeleteProgram(m_id);
    }

    Program::Program(Program&& rhs) noexcept : m_id(std::exchange(rhs.m_id, 0))
    {
    }

    Program& Program::operator=(Program&& rhs) noexcept
    {
        if (this != &rhs) {
            glDeleteProgram(m_id);
            m_id = std::exchange(rhs.m_id, 0);
        }

        return *this;
    }

    Program::operator GLuint() const
    {
        return m_id;
    }

    Shader::Shader(GLenum type) : m_id(glCreateShader(type))
    {
    }

    Shader::~Shader()
    {
        glDeleteShader(m_id);
    }

    Shader::Shader(Shader&& rhs) noexcept : m_id(std::exchange(rhs.m_id, 0))
    {
    }

    Shader& Shader::operator=(Shader&& rhs) noexcept
    {
        if (this != &rhs) {
            glDeleteShader(m_id);
            m_id = std::exchange(rhs.m_id, 0);
        }

        return *this;
    }

    Shader::operator GLuint() const
    {
        return m_id;
    }

    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_id);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_id);
    }

    VertexArray::VertexArray(VertexArray&& rhs) noexcept : m_id(std::exchange(rhs.m_id, 0))
    {
    }

    VertexArray& VertexArray::operator=(VertexArray&& rhs) noexcept
    {
        if (this != &rhs) {
            glDeleteVertexArrays(1, &m_id);
            m_id = std::exchange(rhs.m_id, 0);
        }

        return *this;
    }

    VertexArray::operator GLuint() const
    {
        return m_id;
    }

    void ShaderSource(const Shader& shader, const std::string& source)
    {
        const char* c_str = source.c_str();
        glShaderSource(shader, 1, &c_str, nullptr);
    }

    void CompileShader(const Shader& shader)
    {
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            char info_log[512];
            glGetShaderInfoLog(shader, 512, nullptr, info_log);
            throw std::runtime_error(std::string("Failed to compile shader: ") + info_log);
        }
    }

    void LinkProgram(const Program& program)
    {
        glLinkProgram(program);

        int success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if (!success) {
            char info_log[512];
            glGetProgramInfoLog(program, 512, nullptr, info_log);
            throw std::runtime_error(std::string("Failed to link program: ") + info_log);
        }
    }
}