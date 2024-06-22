#pragma once

#include "../utility/gl_wrapper.h"

class MeshRenderer {
public:
    MeshRenderer();
private:
    GL::Program m_program;
    GL::Texture m_texture;
};
