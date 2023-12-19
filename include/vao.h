#pragma once

#include "vbo.h"

#include "error.h"

#include <vector>

namespace MyGL
{
    struct Vbe
    {
        unsigned int count;
        unsigned int type;
        unsigned char normalized;
        
        inline unsigned int getSize() const
        {
            switch (type)
            {
            case GL_FLOAT: return sizeof(float);
            case GL_UNSIGNED_INT: return sizeof(unsigned int);
            case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
            }
            return 0;
        }
    };

    class Vbl
    {
    public:
        Vbl()
            : m_Stride(0)
        {}

        template<typename T>
        void push(unsigned int count);

        inline const std::vector<Vbe>& getVbes() const { return m_Vbes; }
        inline unsigned int getStride() const { return m_Stride; }

    private:
        std::vector<Vbe> m_Vbes;
        unsigned int m_Stride;
    };

    class Vao
    {
    public:
        Vao();

        void bind() const;
        void unbind() const;

        void addBuffer(const Vbo& vbo, const Vbl& vbl);

        void destroy();

    private:
        unsigned int m_VaoID;
    };
}

#include "vao.hxx"