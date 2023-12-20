#pragma once

#include <GL/glew.h>
#include <iostream>
#include <sstream>
#include <string>

#define CHECK_GL_ERROR() checkGLError(__FILE__, __LINE__)

static bool checkGLError(const std::string& file, int line)
{
    bool errorOccurred = false;
    GLenum errCode;
    do {
        errCode = glGetError();
        if (errCode != GL_NO_ERROR) {
            errorOccurred = true;
            const GLubyte* errString = gluErrorString(errCode);
            std::ostringstream errorMessage;
            errorMessage << "OpenGL Error: [" << errCode << "] " << errString
                         << " at " << file << ":" << line << std::endl;
            std::cerr << errorMessage.str();
        }
    } while (errCode != GL_NO_ERROR);
    return errorOccurred;
}
