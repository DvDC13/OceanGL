#pragma once

#include <GL/glew.h>

#include <iostream>

#define CHECK_GL_ERROR() checkGLError(__FILE__, __LINE__)

static void checkGLError(const char* file, int line)
{
    GLenum errCode;
    const GLubyte *errString;
    while ((errCode = glGetError()) != GL_NO_ERROR)
    {
        errString = gluErrorString(errCode);
        std::cerr << "OpenGL Error: " << errString << " " << file << " " << line << std::endl;
    }
}