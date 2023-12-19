#include "vao.h"

namespace MyGL
{
    Vao::Vao()
    {
        glGenVertexArrays(1, &m_VaoID); CHECK_GL_ERROR();
    }

    void Vao::bind() const
    {
        glBindVertexArray(m_VaoID); CHECK_GL_ERROR();
    }

    void Vao::unbind() const
    {
        glBindVertexArray(0); CHECK_GL_ERROR();
    }

    void Vao::addBuffer(const Vbo& vbo, const Vbl& vbl)
    {
        unsigned int offset = 0;
        const std::vector<Vbe>& vbes = vbl.getVbes();
        for (unsigned int i = 0; i < vbes.size(); i++)
        {
            const Vbe& vbe = vbes[i];
            glVertexAttribPointer(i, vbe.count, vbe.type, vbe.normalized, vbl.getStride(), (const void*)offset); CHECK_GL_ERROR();
            glEnableVertexAttribArray(i); CHECK_GL_ERROR();
            offset += vbe.count * vbe.getSize();
        }
    }

    void Vao::destroy()
    {
        glDeleteVertexArrays(1, &m_VaoID); CHECK_GL_ERROR();
    }
}