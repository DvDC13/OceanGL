#include "vbo.h"

namespace MyGL
{
    Vbo::Vbo()
    {
        glGenBuffers(1, &m_VboID); CHECK_GL_ERROR();
    }

    void Vbo::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VboID); CHECK_GL_ERROR();
    }

    void Vbo::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0); CHECK_GL_ERROR();
    }

    void Vbo::setData(const void* data, unsigned int size, unsigned int usage)
    {
        glBufferData(GL_ARRAY_BUFFER, size, data, usage); CHECK_GL_ERROR();
    }

    void Vbo::destroy()
    {
        glDeleteBuffers(1, &m_VboID); CHECK_GL_ERROR();
    }
}