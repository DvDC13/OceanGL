#pragma once

#include <GL/glew.h>

#include "error.h"

namespace MyGL
{
    class Vbo
    {
    public:
        Vbo();

        void bind() const;
        void unbind() const;

        void setData(const void* data, unsigned int size, unsigned int usage);

        void destroy();

        inline unsigned int getID() const { return m_VboID; }

    private:
        unsigned int m_VboID;
    };
}