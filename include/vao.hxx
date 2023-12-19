#include "vao.h"

namespace MyGL
{
    template<typename T>
    void push(unsigned int count)
    {
        throw std::runtime_error("Unknown type!");
    }

    template<>
    inline void Vbl::push<float>(unsigned int count)
    {
        Vbe vbe = { count, GL_FLOAT, GL_FALSE };
        m_Vbes.push_back(vbe);
        m_Stride += count * vbe.getSize();
    }

    template<>
    inline void Vbl::push<unsigned int>(unsigned int count)
    {
        Vbe vbe = { count, GL_UNSIGNED_INT, GL_FALSE };
        m_Vbes.push_back(vbe);
        m_Stride += count * vbe.getSize();
    }

    template<>
    inline void Vbl::push<unsigned char>(unsigned int count)
    {
        Vbe vbe = { count, GL_UNSIGNED_BYTE, GL_FALSE };
        m_Vbes.push_back(vbe);
        m_Stride += count * vbe.getSize();
    }
}