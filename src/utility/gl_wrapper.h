#pragma once

#include <glad/gl.h>
#include <stdexcept>

namespace GL {
    class Exception : public std::runtime_error {
    public:
        Exception(const std::string& message);
        Exception(const char* message);
    };

    class Buffer {
    public:
        Buffer();
        ~Buffer();

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        Buffer(Buffer&& rhs) noexcept;
        Buffer& operator=(Buffer&& rhs) noexcept;

        operator GLuint() const;
    private:
        GLuint m_id;
    };

    class Program {
    public:
        Program();
        ~Program();

        Program(const Program&) = delete;
        Program& operator=(const Program&) = delete;

        Program(Program&& rhs) noexcept;
        Program& operator=(Program&& rhs) noexcept;

        operator GLuint() const;
    private:
        GLuint m_id;
    };

    class Shader {
    public:
        Shader(GLenum type);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        Shader(Shader&& rhs) noexcept;
        Shader& operator=(Shader&& rhs) noexcept;

        operator GLuint() const;
    private:
        GLuint m_id;
    };

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        VertexArray(VertexArray&& rhs) noexcept;
        VertexArray& operator=(VertexArray&& rhs) noexcept;

        operator GLuint() const;
    private:
        GLuint m_id;
    };

    void ShaderSource(const Shader& shader, const std::string& source);
    void CompileShader(const Shader& shader);
    void LinkProgram(const Program& program);
}
